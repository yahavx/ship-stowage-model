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
#endif

// region Constructors

Simulator::Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir) : travelRootDir(travelRootDir),
                                                                                                                         algorithmsDir(algorithmsDir),
                                                                                                                         outputDir(outputDir),
                                                                                                                         fileDataManager(outputDir,
                                                                                                                                         travelRootDir) {
    // We search those in cwd if not supplied
    if (algorithmsDir == "")
        this->algorithmsDir = ".";
    if (outputDir == "")
        this->outputDir = ".";

#ifndef RUNNING_ON_NOVA
    algorithmFactories.emplace_back([](){return std::make_unique<NaiveStowageAlgorithm>();});
    algorithmNames.push_back("Naive");

//    algorithmFactories.emplace_back([](){return std::make_unique<BadAlgorithm>();});
//    algorithmNames.push_back("Bad");
//
//    algorithmFactories.emplace_back([](){return std::make_unique<RobustStowageAlgorithm>();});
//    algorithmNames.push_back("Robust");
#endif
}

// endregion

// region Simulation run

void Simulator::runSimulations() {
    StringStringVector resultsTable;  // table of results
    Errors generalErrors;

    StringVector travels = fileDataManager.collectLegalTravels(generalErrors);
    fileDataManager.createOutputFolders(generalErrors);
    loadAlgorithmsDynamically(generalErrors);  // We may have no travels to run at this point - but we can collect errors
    generalErrors.addSimulatorInitLog();

    initResultsTable(resultsTable, travels, algorithmNames);  // add columns names and set table structure

    for (auto &travel: travels) {
        fileDataManager.setTravelName(extractFilenameFromPath(travel));

        for (longUInt i = 0; i < algorithmFactories.size(); i++) {
            std::unique_ptr<AbstractAlgorithm> algorithm = algorithmFactories[i]();
            fileDataManager.setAlgorithmName(algorithmNames[i]);
            fileDataManager.createTravelCraneFolder();

            int totalCraneInstructions = runSimulation(std::move(algorithm));

            addSimulationResultToTable(resultsTable, totalCraneInstructions, i + 1);
        }
    }

    if (!travels.empty()) {
        finalizeResultsTable(resultsTable);
        fileDataManager.saveSimulationResults(resultsTable);
    }

    if (generalErrors.hasErrors()) {
        fileDataManager.saveGeneralErrors(generalErrors);
    }

    fileDataManager.cleanOutputFolders();  // remove temp and errors (if empty)
}

int Simulator::runSimulation(std::unique_ptr<AbstractAlgorithm> algorithm) {
    Errors errors;

    // region Init

    std::cout << "Starting simulation (Algorithm = " << fileDataManager.algorithmName << ", Travel = " << fileDataManager.travelName << ")" << std::endl;

    WeightBalanceCalculator simWeightBalancer;
    ContainerShip ship = initSimulationShip(simWeightBalancer, errors);

    WeightBalanceCalculator algoWeightBalancer;
    Error algorithmReport = initAlgorithmShip(algorithm.get(), algoWeightBalancer);

    if (algorithmReport.isFatalError()) {  // Algorithm bad report - the travel wouldn't run if it was the case

    }

    StringToStringVectorMap cargoData = fileDataManager.getCargoDataFiles(errors);  // get list of .cargo_data files, ordered for each port
    StringToIntMap portsVisits = initPortsVisits(ship.getShipRoute());  // map from each port, to number of times we have encountered him so far
    filterUnusedPorts(cargoData, ship.getShipRoute(), errors);  // remove the port files which are not on the ship route

    errors.addSimulationInitLog();

    // endregion

    #ifdef DEBUG_PRINTS
    std::cout << "The ship has started its journey!" << std::endl;
    printSeparator(1, 1);
    #endif

    std::vector<PortId> ports = ship.getShipRoute().getPorts();
    int totalNumberOfOps = 0;
    for (longUInt i = 0; i < ports.size(); i++) {  // Start the journey
        auto &portId = ports[i];
        int visitNum = getVisitNum(portsVisits, portId);  // visit number at this port right now
        bool isLast = (i == ports.size() - 1);  // our last port is treated a bit different

        #ifdef DEBUG_PRINTS
        std::cout << "The ship has docked at port " << portId << "." << std::endl;
        #endif

        std::string cargoFileName = getNextFileForPort(cargoData, portsVisits, portId, fileDataManager, isLast);
        std::string cargoFilePath = fileDataManager.cargoFilePath(cargoFileName);
        std::string instructionsOutputPath = fileDataManager.craneInstructionsOutputPath(portId, visitNum);
        algorithm->getInstructionsForCargo(cargoFilePath, instructionsOutputPath);

        Port port(portId, readPortCargoFromFile(cargoFilePath, errors));  // init current port for the simulator

        if (isLast && !port.getStorage().isEmpty()) { // Last port has containers
            errors.addError({ErrorFlag::ContainersAtPort_LastPortHasContainers});
        }

        Operations ops = readPackingOperationsFromFile(instructionsOutputPath, errors);  // read the operations to perform, written by the algorithm
        performPackingOperations(ship, port, ops, errors);
        totalNumberOfOps = totalNumberOfOps + ops.size(true);

        errors.addSimulationPortVisitLog(visitNum, ports[i], i + 1);

        if (errors.hasAlgorithmErrors()) {
            reportSimulationError(errors);
            return -1;
        }

        #ifdef DEBUG_PRINTS
        if (!isLast) {
            std::cout << "The ship is continuing to the next port..." << std::endl;
        } else { std::cout << "The ship is going into maintenance..." << std::endl; }
        printSeparator(1, 1);
        #endif
    }

    validateNoCargoFilesLeft(cargoData, errors);  // if there are remaining cargo files in the map, we need to print a warning because we couldn't use them

    #ifdef DEBUG_PRINTS
    std::cout << "The ship has completed its journey. Total number of operations: " << totalNumberOfOps << std::endl;
    printSeparator(1, 3);
    #endif

    if (errors.hasErrors()) {
        fileDataManager.saveSimulationErrors(errors);
    }

    return errors.hasAlgorithmErrors() ? -1 : totalNumberOfOps;  // We can't really return -1 from here - maybe in the future
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

// endregion

// region Simulation Init



int Simulator::initAlgorithmShip(AbstractAlgorithm *algorithm, WeightBalanceCalculator &calculator) {
    int ret = algorithm->readShipPlan(fileDataManager.shipPlanPath());
    int ret2 = algorithm->readShipRoute(fileDataManager.shipRoutePath());
    int ret3 = algorithm->setWeightBalanceCalculator(calculator);

    return ret | ret2 | ret3;

    // TODO: below we check if algorithm report matches to what we except, and penalise him if yes. Decided to abandon it for this exercise (risky).
//    int errorsDiff = errors.compareReports(algorithmErrors);
//
//    if (errorsDiff > 0) {
//        errors.addError({ErrorFlag::AlgorithmError_MissingReport, intToStr(errorsDiff)});
//    }
//
//    if (errorsDiff < 0) {
//        errors.addError({ErrorFlag::AlgorithmError_ExtraReport, intToStr(-errorsDiff)});
//    }
}

ContainerShip Simulator::initSimulationShip(WeightBalanceCalculator &calculator, Errors &errors) {
    ShipPlan shipPlan = readShipPlanFromFile(fileDataManager.shipPlanPath(), errors);
    ShipRoute shipRoute = readShipRouteFromFile(fileDataManager.shipRoutePath(), errors);

    return ContainerShip(shipPlan, shipRoute, calculator);
}

// endregion

// region Perform operations and validations

bool Simulator::performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, Errors &errors) const { // Perform operations on local ship and port

    StringVector badContainers = port.removeBadContainers(ship.getShipRoute(), errors);  // Removes from port and returns the ids of the bad containers
    AlgorithmValidation validation(ship, port, badContainers, errors);
    CranesManagement crane(ship, port);

    for (const PackingOperation &op : ops.ops) {

        if (!validation.validatePackingOperation(op)) {
            return false;
        }

        if (op.getType() == PackingType::reject)
            continue;

        auto opResult = crane.preformOperation(op);
        if (opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND) {
            #ifdef DEBUG_PRINTS
            std::cout << "crane received illegal operation, didn't find container with ID: " << op.getContainerId() << std::endl;
            #endif
            errors.addError({ErrorFlag::AlgorithmError_CraneOperationWithInvalidId, op.getContainerId(), port.getId(), op.toString()});
        }
        if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP) {
            #ifdef DEBUG_PRINTS
            std::cout << "Illegal crane operation: " << op << std::endl;
            #endif
            errors.addError({ErrorFlag::AlgorithmError_InvalidCraneOperation, op.toString()});
        }
    }

    ship.advanceToNextPort();

    if (!validation.validateNoContainersLeftOnPort()) {
        return false;
    }

    std::cout << ops;
    return true;
}

// endregion

// region Simulation finish

void Simulator::reportSimulationError(Errors &errors) {
    #ifdef DEBUG_PRINTS
    std::cout << "Found an error in the algorithm, terminating" << std::endl << errors;
    printSeparator(1, 3);
    #endif
    errors.addSimulationErrorLog();
    fileDataManager.saveSimulationErrors(errors);
}

// endregion

// region Constants

const std::string Simulator::s_resultsTableTitle = "RESULTS";
const std::string Simulator::s_sumColumnTitle = "Sum";

const std::string Simulator::s_errorsColumnTitle = "Num Errors";

// endregion
