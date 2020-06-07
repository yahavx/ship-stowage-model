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
#include "SimulationManager.h"

#ifndef RUNNING_ON_NOVA

#include "../algorithms/NaiveStowageAlgorithm.h"
#include "../algorithms/BadAlgorithm.h"
#include "../algorithms/RobustStowageAlgorithm.h"
#include "SimpleTaskProducer.h"
#include "../common/utils/strongTypes.h"
#include "AlgorithmTravelTask.h"
#include "ThreadPoolExecuter.h"

#endif

// region Constructors

Simulator::Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir, const int numThreads) : travelRootDir(travelRootDir),
                                                                                                                         algorithmsDir(algorithmsDir),
                                                                                                                         outputDir(outputDir),
                                                                                                                         numThreads(numThreads),
                                                                                                                         rootFileManager(outputDir,
                                                                                                                                         travelRootDir) {
#ifndef RUNNING_ON_NOVA
    algorithmFactories.emplace_back([]() { return std::make_unique<NaiveStowageAlgorithm>(); });
    algorithmNames.push_back("Naive");

//    algorithmFactories.emplace_back([](){return std::make_unique<BadAlgorithm>();});
//    algorithmNames.push_back("Bad");
//
    algorithmFactories.emplace_back([]() { return std::make_unique<RobustStowageAlgorithm>(); });
    algorithmNames.push_back("Robust");
#endif
}

Simulator::Simulator(const std::string &travelRootDir, int numThreads, const std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &algorithmFactories,
                     const std::string &outputDir) : travelRootDir(travelRootDir), outputDir(outputDir), numThreads(numThreads),  rootFileManager(outputDir, travelRootDir),
                                                     algorithmFactories(algorithmFactories) {}


// endregion

// region Simulation run

void Simulator::runSimulations() {
    tracer.traceVerbose("Simulator started.", true);
    StringStringVector resultsTable;  // table of results
    Errors generalErrors;

    StringVector travels = rootFileManager.collectLegalTravels(generalErrors);
    rootFileManager.createOutputFolders(generalErrors);
    loadAlgorithmsDynamically(generalErrors);  // We may have no travels to run at this point - but we can collect errors
    generalErrors.addSimulatorInitLog();

    initResultsTableWithPlaceholders(resultsTable, travels, algorithmNames);  // Add columns names and set table structure

    // Create producer for all algorithm-travel pair tasks
//    SimpleTasksProducer producer = createAlgorithmTravelTasksProducer(travels, resultsTable);

    auto tasks = createAlgorithmTravelTasksProducer(travels, resultsTable);

    ThreadPoolExecuter executor {
            // Create producer for all algorithm-travel pair tasks
            SimpleTasksProducer(tasks),
            NumThreads{numThreads}
    };

    // Start running all the tasks
    executor.start();

    // Wait until all tasks are finished
    executor.wait_till_finish();

//    for (auto &travel: travels) {
//        SimulatorFileManager fileManager(outputDir, travelRootDir);
//        fileManager.setTravelName(extractFilenameFromPath(travel));
//
//        for (longUInt i = 0; i < algorithmFactories.size(); i++) {
//            tracer.traceVerbose("Creating instance of algorithm " + algorithmNames[i]);
//            std::unique_ptr<AbstractAlgorithm> algorithm = algorithmFactories[i]();
//            fileManager.setAlgorithmName(algorithmNames[i]);
//            fileManager.createTravelCraneFolder();
//
//            tracer.traceVerbose("Starting a simulation.");
//            int totalCraneInstructions = runSimulation(fileManager, std::move(algorithm));
//
//            addSimulationResultToTable(resultsTable, totalCraneInstructions, i + 1);
//        }
//    }

    if (!travels.empty() && !algorithmFactories.empty()) {
        finalizeResultsTable(resultsTable);
        rootFileManager.saveSimulationResults(resultsTable);
    } else {
        tracer.traceInfo("No legal travels and/or no algorithms were available (no simulations were ran).");
    }

    if (generalErrors.hasErrors()) {
        rootFileManager.saveGeneralErrors(generalErrors);
    }

    rootFileManager.cleanOutputFolders();  // remove temp and errors (if empty)
}

std::vector<AlgorithmTravelTask> Simulator::createAlgorithmTravelTasksProducer(StringVector &travels, StringStringVector &resultsTable) {
    std::vector<AlgorithmTravelTask> tasks;

    for (longUInt t = 0; t < travels.size(); t++) {
        auto &travel = travels[t];
        for (longUInt a = 0; a < algorithmFactories.size(); a++) {
            SimulatorFileManager fileManager(outputDir, travelRootDir);
            fileManager.setTravelName(extractFilenameFromPath(travel));

            tracer.traceVerbose("Creating instance of algorithm " + algorithmNames[a]);

            fileManager.setAlgorithmName(algorithmNames[a]);
            fileManager.createTravelCraneFolder();

            AlgorithmTravelTask task(fileManager, tracer, resultsTable, std::pair(a, t), algorithmFactories[a], travel);
            tasks.push_back(task);

            tracer.traceVerbose("created task: " + algorithmNames[a] + ", " + travel);
        }
    }

    return tasks;
}

//void Simulator::runSimulations() {
//    tracer.traceVerbose("Simulator started.", true);
//    StringStringVector resultsTable;  // table of results
//    Errors generalErrors;
//
//    StringVector travels = rootFileManager.collectLegalTravels(generalErrors);
//    rootFileManager.createOutputFolders(generalErrors);
//    loadAlgorithmsDynamically(generalErrors);  // We may have no travels to run at this point - but we can collect errors
//    generalErrors.addSimulatorInitLog();
//
//    initResultsTable(resultsTable, travels, algorithmNames);  // Add columns names and set table structure
//
//    for (auto &travel: travels) {
//        SimulatorFileManager fileManager(outputDir, travelRootDir);
//        fileManager.setTravelName(extractFilenameFromPath(travel));
//
//        for (longUInt i = 0; i < algorithmFactories.size(); i++) {
//            tracer.traceVerbose("Creating instance of algorithm " + algorithmNames[i]);
//            std::unique_ptr<AbstractAlgorithm> algorithm = algorithmFactories[i]();
//            fileManager.setAlgorithmName(algorithmNames[i]);
//            fileManager.createTravelCraneFolder();
//
//            tracer.traceVerbose("Starting a simulation.");
//            int totalCraneInstructions = runSimulation(fileManager, std::move(algorithm));
//
//            addSimulationResultToTable(resultsTable, totalCraneInstructions, i + 1);
//        }
//    }
//
//    if (!travels.empty() && !algorithmFactories.empty()) {
//        finalizeResultsTable(resultsTable);
//        rootFileManager.saveSimulationResults(resultsTable);
//    }
//    else {
//        tracer.traceInfo("No legal travels and/or no algorithms were available (no simulations were ran).");
//    }
//
//    if (generalErrors.hasErrors()) {
//        rootFileManager.saveGeneralErrors(generalErrors);
//    }
//
//    rootFileManager.cleanOutputFolders();  // remove temp and errors (if empty)
//}

void Simulator::loadAlgorithmsDynamically(Errors &errors) {
#ifndef RUNNING_ON_NOVA
    return;
#endif
    tracer.traceVerbose("Looking for algorithms...", true);

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

        tracer.traceVerbose("Loading file: " + file);

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

    tracer.separator(TraceVerbosity::Verbose, 0, 0);
    tracer.traceVerbose("Finished looking for algorithms.", true);

    if (algorithmFactories.empty()) {
        errors.addError(ErrorFlag::SharedObject_NoAlgorithmsLoaded);
    }
}

int Simulator::runSimulation(SimulatorFileManager &fileManager, std::unique_ptr<AbstractAlgorithm> algorithm) {
    SimulationManager simManager(fileManager, tracer);

    // region Init

    tracer.traceInfo("Starting simulation (Algorithm = " + fileManager.algorithmName + ", Travel = " + fileManager.travelName + ")", true);

    WeightBalanceCalculator weightBalancer, algoWeightBalancer;

    simManager.initSimulationShip(weightBalancer);
    bool success = simManager.initAlgorithmShip(algorithm.get(), algoWeightBalancer);
    simManager.initCargoData();  // order the files for each port, filter irrelevant files

    if (!success) {  // Algorithm failed to initialize
        tracer.traceInfo("Algorithm failed to initialize, terminating.");
        tracer.separator(TraceVerbosity::Info, 0, 3);
        simManager.saveErrors();
        return -1;
    }

    // endregion

    tracer.traceInfo("The ship has started its journey!", true);

    for (auto &portId : simManager.getRoutePorts()) {  // Start the journey
        tracer.traceInfo("The ship has docked at port " + portId.getCode());

        std::string instructionsOutputPath = simManager.getInstructionsForCargo(algorithm.get());
        success = simManager.performPackingOperations(instructionsOutputPath);

        if (!success) {
            simManager.saveErrors();
            return -1;
        }

        std::string message = simManager.isRouteFinished() ? "The ship is going into maintenance..." : "The ship is continuing to the next port...";
        tracer.traceInfo(message, true);
    }

    int totalNumberOfOps = simManager.finishSimulation();

    tracer.traceInfo("The ship has completed its journey. Total number of operations: " + intToStr(totalNumberOfOps));
    tracer.separator(TraceVerbosity::Info, 0, 3);

    return totalNumberOfOps;
}

// endregion

// region Simulation finish



// endregion

// region Constants

const std::string Simulator::s_resultsTableTitle = "RESULTS";
const std::string Simulator::s_sumColumnTitle = "Sum";
const std::string Simulator::s_errorsColumnTitle = "Num Errors";
const std::string Simulator::s_resultsTablePlaceholder = "-1";


// endregion
