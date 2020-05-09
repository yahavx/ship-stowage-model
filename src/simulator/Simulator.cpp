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

#ifndef RUNNING_ON_NOVA
#include "../algorithms/NaiveStowageAlgorithm.h"
#include "../algorithms/BadAlgorithm.h"
#include "../algorithms/RobustStowageAlgorithm.h"
#endif
#ifdef RUNNING_ON_NOVA
#include <dlfcn.h>
#endif

// region Constructors

Simulator::Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir) : travelRootDir(travelRootDir),
                                                                                                                         algorithmsDir(algorithmsDir),
                                                                                                                         outputDir(outputDir),
                                                                                                                         dataManager(outputDir,
                                                                                                                                     travelRootDir) {
#ifndef RUNNING_ON_NOVA
    auto naiveStowageAlgorithm = std::make_shared<NaiveStowageAlgorithm>();
    auto badAlgorithm = std::make_shared<BadAlgorithm>();
    auto robustAlgorithm = std::make_shared<RobustStowageAlgorithm>();

    algorithms.push_back(naiveStowageAlgorithm);
    algorithms.push_back(badAlgorithm);
    algorithms.push_back(robustAlgorithm);
#endif
}

// endregion

// region Simulation run

void Simulator::runSimulations() {
    StringStringVector resultsTable;  // table of results
    Errors generalErrors;

    dataManager.createOutputFolders(generalErrors);

    StringVector travels = dataManager.collectLegalTravels(generalErrors);
    initResultsTable(resultsTable, travels, algorithms);  // add columns names and set table structure
#ifdef RUNNING_ON_NOVA
    loadAlgorithmsDynamically(generalErrors);
#endif

    for (auto &travel: travels) {
        dataManager.setTravelName(extractFilenameFromPath(travel));

        for (longUInt i = 0; i < algorithms.size(); i++) {
            auto &algorithm = algorithms[i];
#ifndef RUNNING_ON_NOVA
            dataManager.setAlgorithmName(algorithm->getAlgorithmName());
#else
            dataManager.setAlgorithmName("Algorithm" + intToStr(i + 1));
#endif
            dataManager.createTravelCraneFolder();

            int totalCraneInstructions = runSimulation(*algorithm);

            addSimulationResultToTable(resultsTable, totalCraneInstructions, i + 1);
        }
    }

    if (!travels.empty()) {
        finalizeResultsTable(resultsTable);
        dataManager.saveSimulationResults(resultsTable);
    }

    if (generalErrors.hasErrors()) {
        dataManager.saveGeneralErrors(generalErrors);
    }

    dataManager.cleanOutputFolders();  // remove temp and errors (if empty), can disable it to debug
}

int Simulator::runSimulation(AbstractAlgorithm &algorithm) {
    Errors errors;

    // region Init

    ////////////////////////
    ///     Init         ///
    ////////////////////////

    std::cout << "Starting simulation (Algorithm = " << dataManager.algorithmName << ", Travel = " << dataManager.travelName << ")" << std::endl;

    WeightBalanceCalculator simWeightBalancer;
    ContainerShip ship = initSimulation(simWeightBalancer, errors);

    WeightBalanceCalculator algoWeightBalancer;
    initAlgorithm(algorithm, algoWeightBalancer, errors);

    StringToStringVectorMap cargoData = dataManager.getCargoDataFiles(errors);  // get list of .cargo_data files, ordered for each port
    StringToIntMap portsVisits = initPortsVisits(ship.getShipRoute());  // map from each port, to number of times we have encountered him so far
    filterUnusedPorts(cargoData, ship.getShipRoute(), errors);  // remove the port files which are not on the ship route

    errors.addSimulationInitLog();

    ////////////////////////
    /// Start simulation ///
    ////////////////////////

    std::cout << "The ship has started its journey!" << std::endl;
    printSeparator(1, 1);

    std::vector<PortId> ports = ship.getShipRoute().getPorts();
    int totalNumberOfOps = 0;
    for (longUInt i = 0; i < ports.size(); i++) {  // Start the journey
        auto &portId = ports[i];
        int visitNum = getVisitNum(portsVisits, portId);  // visit number at this port right now
        bool isLast = (i == ports.size() - 1);  // our last port is treated a bit different

        std::cout << "The ship has docked at port " << portId << "." << std::endl;

        std::string cargoFileName = getNextFileForPort(cargoData, portsVisits, portId, dataManager, isLast);
        std::string cargoFilePath = dataManager.cargoFilePath(cargoFileName);
        std::string instructionsOutputPath = dataManager.craneInstructionsOutputPath(portId, visitNum);
        algorithm.getInstructionsForCargo(cargoFilePath, instructionsOutputPath);

        Port port(portId, readPortCargoFromFile(cargoFilePath, errors));  // init current port for the simulator

        if(isLast && !port.getStorage().isEmpty()) { // Last port has containers
            errors.addError({ErrorFlag::ContainersAtPort_LastPortHasContainers});
        }

        Operations ops = readPackingOperationsFromFile(instructionsOutputPath, errors);  // read the operations to perform, written by the algorithm
        performPackingOperations(ship, port, ops, errors);
        totalNumberOfOps = totalNumberOfOps + ops.size(true);

        if (errors.hasAlgorithmErrors()) {
            std::cout << "Found an error in the algorithm, terminating" << std::endl << errors;
            printSeparator(1, 3);
            errors.addSimulationPortVisitLog(visitNum, ports[i], i + 1);
            errors.addSimulationErrorLog();
            dataManager.saveSimulationErrors(errors);
            return -1;
        }

        if (!isLast) {
            std::cout << "The ship is continuing to the next port..." << std::endl;
        } else { std::cout << "The ship is going into maintenance..." << std::endl; }

        printSeparator(1, 1);
        errors.addSimulationPortVisitLog(visitNum, ports[i], i + 1);
    }

    validateNoCargoFilesLeft(cargoData, errors);  // if there are remaining cargo files in the map, we need to print a warning because we couldn't use them

//    printSeparator(1, 1);

    std::cout << "The ship has completed its journey. Total number of operations: " << totalNumberOfOps << std::endl;
    printSeparator(1, 3);

    if (errors.hasErrors()) {
        dataManager.saveSimulationErrors(errors);
    }

    return errors.hasAlgorithmErrors() ? -1 : totalNumberOfOps;
}

#ifdef RUNNING_ON_NOVA
struct DlCloser{
    void operator()(void *dlHandle) const noexcept {
        dlclose(dlHandle);
    }
};

void Simulator::loadAlgorithmsDynamically(Errors &errors) {
    if (algorithmsDir == "") {
        algorithmsDir = ".";  // We search in current directory if no path supplied
    }

    // TODO

    if (!isDirectoryExists(algorithmsDir)) {

    }

    if (isFolderEmpty(algorithmsDir)){

    }

    (void)errors;
}
#endif

// endregion

// region Simulation Init

void Simulator::initAlgorithm(AbstractAlgorithm &algorithm, WeightBalanceCalculator &calculator, Errors &errors) {
    int ret = algorithm.readShipPlan(dataManager.shipPlanPath());
    errors.addError(ret);  // if its not an error, addError will ignore it

    ret = algorithm.readShipRoute(dataManager.shipRoutePath());
    errors.addError(ret);

    ret = algorithm.setWeightBalanceCalculator(calculator);
    errors.addError(ret);
}

ContainerShip Simulator::initSimulation(WeightBalanceCalculator &calculator, Errors &errors) {
    ShipPlan shipPlan = readShipPlanFromFile(dataManager.shipPlanPath(), errors);
    ShipRoute shipRoute = readShipRouteFromFile(dataManager.shipRoutePath(), errors);

    return ContainerShip(shipPlan, shipRoute, calculator);
}

// endregion

// region Perform operations and validations

void
Simulator::performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, Errors &errors) const { // Perform operations on local ship and port

    // TODO: check that any containers that were loaded to the port to unload others, are back in ship
    // TODO: ops can be empty, maybe we need to document it

    StringVector badContainers = port.removeBadContainers(ship.getShipRoute());  // Removes from port and returns the ids of the bad containers
    AlgorithmValidation validation(ship, port, badContainers, errors);
    CranesManagement crane(ship, port);

    for (const PackingOperation &op : ops.ops) {

        validation.validatePackingOperation(op);  // TODO: add support + checks to all 4 commands

        if (errors.hasAlgorithmErrors())
            return;

        if (op.getType() == PackingType::reject)
            continue;

        auto opResult = crane.preformOperation(op);
        if (opResult ==
            CraneOperationResult::FAIL_CONTAINER_NOT_FOUND) {  // TODO: this should generally always succeed - we need to validate before sending inst. to crane
            std::cout << "crane received illegal operation, didn't find container with ID: " << op.getContainerId() << std::endl;
            errors.addError({ErrorFlag::AlgorithmError_CraneOperationWithInvalidId, op.getContainerId(), port.getId(), op.toString()});
        }
        if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP) {
            std::cout << "Illegal crane operation: " << op << std::endl;
            errors.addError({ErrorFlag::AlgorithmError_InvalidCraneOperation, op.toString()});
        }
    }

    ship.advanceToNextPort();

    validation.validateNoContainersLeftOnPort();

    std::cout << ops;
}

// endregion

// region Constants

const std::string Simulator::s_resultsTableTitle = "RESULTS";
const std::string Simulator::s_sumColumnTitle = "Sum";

const std::string Simulator::s_errorsColumnTitle = "Num Errors";

// endregion
