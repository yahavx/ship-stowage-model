//
// Created by t-yabeny on 4/18/2020.
//

#include <iostream>
#include "Simulator.h"
#include "SimulatorUtil.h"
#include "AlgorithmValidation.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/Printers.h"
#include "../common/actors/CranesManagement.h"
#include "../common/utils/Errors.h"
#include "../common/io/FileReader.h"
#include "../common/utils/UtilFunctions.h"
#include "../interfaces/AbstractAlgorithm.h"
#include "AlgorithmRegistrar.h"

#ifndef RUNNING_ON_NOVA

#include "../algorithms/NaiveStowageAlgorithm.h"
#include "../algorithms/BadAlgorithm.h"
#include "../algorithms/RobustStowageAlgorithm.h"
#include "SimulationManager.h"

#endif

// region Constructors

Simulator::Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir) : travelRootDir(travelRootDir),
                                                                                                                         algorithmsDir(algorithmsDir),
                                                                                                                         outputDir(outputDir),
                                                                                                                         fileManager(outputDir,
                                                                                                                                     travelRootDir) {
    // We search those in cwd if not supplied
    if (algorithmsDir == "")
        this->algorithmsDir = ".";
    if (outputDir == "")
        this->outputDir = ".";

#ifndef RUNNING_ON_NOVA
    algorithmFactories.emplace_back([]() { return std::make_unique<NaiveStowageAlgorithm>(); });
    algorithmNames.push_back("Naive");

//    algorithmFactories.emplace_back([](){return std::make_unique<BadAlgorithm>();});
//    algorithmNames.push_back("Bad");
//
//    algorithmFactories.emplace_back([](){return std::make_unique<RobustStowageAlgorithm>();});
//    algorithmNames.push_back("Robust");
#endif
}

Simulator::Simulator(const std::string &travelRootDir, const std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &algorithmFactories,
                     const std::string &outputDir) : travelRootDir(travelRootDir), outputDir(outputDir), fileManager(outputDir, travelRootDir),
                                                     algorithmFactories(algorithmFactories) {}


// endregion

// region Simulation run

void Simulator::runSimulations() {
    StringStringVector resultsTable;  // table of results
    Errors generalErrors;

    StringVector travels = fileManager.collectLegalTravels(generalErrors);
    fileManager.createOutputFolders(generalErrors);
    loadAlgorithmsDynamically(generalErrors);  // We may have no travels to run at this point - but we can collect errors
    generalErrors.addSimulatorInitLog();

    initResultsTable(resultsTable, travels, algorithmNames);  // Add columns names and set table structure

    for (auto &travel: travels) {
        fileManager.setTravelName(extractFilenameFromPath(travel));

        for (longUInt i = 0; i < algorithmFactories.size(); i++) {
            std::unique_ptr<AbstractAlgorithm> algorithm = algorithmFactories[i]();
            fileManager.setAlgorithmName(algorithmNames[i]);
            fileManager.createTravelCraneFolder();

            int totalCraneInstructions = runSimulation(std::move(algorithm));

            addSimulationResultToTable(resultsTable, totalCraneInstructions, i + 1);
        }
    }

    if (!travels.empty()) {
        finalizeResultsTable(resultsTable);
        fileManager.saveSimulationResults(resultsTable);
    }

    if (generalErrors.hasErrors()) {
        fileManager.saveGeneralErrors(generalErrors);
    }

    fileManager.cleanOutputFolders();  // remove temp and errors (if empty)
}

void Simulator::loadAlgorithmsDynamically(Errors &errors) {
#ifndef RUNNING_ON_NOVA
    return;
#endif
    if (!isDirectoryExists(algorithmsDir)) {
        errors.addError({ErrorFlag::SharedObject_InvalidDirectory, algorithmsDir});
        return;
    }

    if (isFolderEmpty(algorithmsDir)) {
        errors.addError({ErrorFlag::SharedObject_InvalidDirectory, algorithmsDir});
        return;
    }

    auto files = getFilesFromDirectory(algorithmsDir);

    auto &registrar = AlgorithmRegistrar::getInstance();

    for (auto &file: files) {
        if (!endsWith(file, ".so")) {
            continue;
        }

        ErrorFlag soLoadResult = registrar.loadSharedObject(file);
        if (soLoadResult != ErrorFlag::Success) {
            errors.addError({soLoadResult, extractFilenameFromPath(file)});
            continue;
        }

        int delta = registrar.factoriesIncrease();  // how many algorithms were added

        switch (delta) {
            case 0:
                errors.addError({ErrorFlag::SharedObject_AlgorithmDidntSelfRegister, extractFilenameFromPath(file)});
                break;
            case 1:  // The expected behaviour
                algorithmFactories.push_back(registrar.getLast());
                algorithmNames.push_back(extractFilenameFromPath(file, true));
                break;
            default:
                errors.addError({ErrorFlag::SharedObject_LoadedMoreThanOneAlgorithm, extractFilenameFromPath(file)});
                break;
        }
    }

    if (algorithmFactories.empty()) {
        errors.addError(ErrorFlag::SharedObject_NoAlgorithmsLoaded);
    }
}

int Simulator::runSimulation(std::unique_ptr<AbstractAlgorithm> algorithm) {
    SimulationManager simManager(fileManager);

    // region Init

#ifdef DEBUG_PRINTS
    std::cout << "Starting simulation (Algorithm = " << fileManager.algorithmName << ", Travel = " << fileManager.travelName << ")" << std::endl;
#endif

    WeightBalanceCalculator weightBalancer, algoWeightBalancer;

    simManager.initSimulationShip(weightBalancer);
    bool success = simManager.initAlgorithmShip(algorithm.get(), algoWeightBalancer);

    if (!success) {  // Algorithm failed to initialize
        simManager.saveErrors();
        return -1;
    }

    simManager.initCargoData();

    // endregion

#ifdef DEBUG_PRINTS
    std::cout << "The ship has started its journey!" << std::endl;
    printSeparator(1, 1);
#endif

    for (auto &portId : simManager.getRoutePorts()) {  // Start the journey
#ifdef DEBUG_PRINTS
        std::cout << "The ship has docked at port " << portId << "." << std::endl;
#endif

        std::string instructionsOutputPath = simManager.getInstructionsForCargo(algorithm.get());
        success = simManager.performPackingOperations(instructionsOutputPath);

        if (!success) {
            simManager.saveErrors();
            return -1;
        }

#ifdef DEBUG_PRINTS
        if (!simManager.isCurrentLastPort()) {
            std::cout << "The ship is continuing to the next port..." << std::endl;
        } else { std::cout << "The ship is going into maintenance..." << std::endl; }
        printSeparator(1, 1);
#endif
    }

    int totalNumberOfOps = simManager.finishSimulation();

#ifdef DEBUG_PRINTS
    std::cout << "The ship has completed its journey. Total number of operations: " << totalNumberOfOps << std::endl;
    printSeparator(1, 3);
#endif

    return totalNumberOfOps;
}

// endregion

// region Simulation finish



// endregion

// region Constants

const std::string Simulator::s_resultsTableTitle = "RESULTS";
const std::string Simulator::s_sumColumnTitle = "Sum";

const std::string Simulator::s_errorsColumnTitle = "Num Errors";

// endregion
