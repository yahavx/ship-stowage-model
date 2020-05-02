//
// Created by t-yabeny on 4/18/2020.
//

#include "Simulator.h"
#include "SimulatorUtil.h"
#include "../algorithms/stowage/NaiveStowageAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/Printers.h"
#include "../algorithms/CranesOperation.h"
#include "../common/io/FileReader.h"
#include "../common/utils/UtilFunctions.h"
#include "../common/utils/Errors.h"
#include "../algorithms/stowage/BadAlgorithm.h"

// region Constants

const std::string Simulator::s_instructionsFilename = "cargo_instructions";

const std::string Simulator::s_resultsTableTitle = "RESULTS";
const std::string Simulator::s_generalErrorsTableName = "GeneralErrors";

const std::string Simulator::s_sumColumnTitle = "Sum";
const std::string Simulator::s_errorsColumnTitle = "Num Errors";

const std::string Simulator::s_errorToken = "Error";

const std::string Simulator::s_noTravelPathSuppliedError = "Travel path was not supplied";
// endregion

// region Constructors

Simulator::Simulator(const std::string &outputDir, const std::string &travelRootDir) : outputDir(outputDir), travelRootDir(travelRootDir),
                                                                                       dataManager(outputDir, travelRootDir) {

    auto naiveStowageAlgorithm = std::make_shared<NaiveStowageAlgorithm>();
    auto badAlgorithm = std::make_shared<BadAlgorithm>();
//    NaiveStowageAlgorithm *naiveStowageAlgorithm2 = new NaiveStowageAlgorithm();

    algorithms.push_back(naiveStowageAlgorithm);
    algorithms.push_back(badAlgorithm);
//    algorithms.push_back(naiveStowageAlgorithm2);
}
// endregion

// region Simulation run

void Simulator::runSimulations() {
    StringStringVector results;  // table for results
    Errors generalErrors;

    dataManager.createOutputFolders(generalErrors);
    StringVector travels = dataManager.collectTravels(generalErrors);

    if (!generalErrors.hasNoErrors()) {  // any init error is fatal so we have to terminate
        dataManager.saveErrorFile("GeneralErrors", generalErrors);
        dataManager.cleanOutputFolders();
        return;
    }

    initResultsTable(results, travels, algorithms);  // add columns names and set table structure
    for (auto &travel: travels) {

        dataManager.setTravelName(extractFilenameFromPath(travel));

        if (!isTravelValid(dataManager)) {
            continue;
        }

        for (longUInt i = 0; i < algorithms.size(); i++) {
            auto &algorithm = algorithms[i];

            dataManager.setAlgorithmName(algorithm->getAlgorithmName());
            dataManager.createTravelCraneFolder();

            StringStringVector simulationResults = runSimulation(*algorithm);  // run simulation, collect data

            addSimulationResultToTable(simulationResults, results, i + 1);  // save collected data from the travel to the tables
        }
    }

    finalizeResultsTable(results);

    dataManager.saveSimulationTables(results, generalErrors);

    dataManager.cleanOutputFolders();  // remove temp and errors (if empty), can disable it to debug
}

StringStringVector Simulator::runSimulation(AbstractAlgorithm &algorithm) {
    StringStringVector report = StringStringVector(2, StringVector());
    StringVector &results = report[0];
    Errors errors;

    ////////////////////////
    ///     Init         ///
    ////////////////////////

    // Get plan and route paths
    std::string shipPlanPath = dataManager.shipPlanPath();
    std::string shipRoutePath = dataManager.shipRoutePath();

    // Init for simulation
    ContainerShip ship = initSimulation(shipPlanPath, shipRoutePath, errors);

    // Init for algorithm
    initAlgorithm(algorithm, shipPlanPath, shipRoutePath);

    StringToStringVectorMap cargoData = sortTravelCargoData(dataManager.travelFolder());  // get list of .cargo_data files, ordered for each port
    StringToIntMap portsVisits = initPortsVisits(ship.getShipRoute());  // map from each port, to number of times we have encountered him so far

    std::cout << "Validating route..." << std::endl;
    filterUnusedPorts(cargoData, ship.getShipRoute());  // remove the port files which are not on the ship route
    std::cout << "Finished." << std::endl;

    printSeparator(1, 1);

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

        std::cout << "The ship has docked at port " << portId.getCode() << "." << std::endl;

        bool isLast = (i == ports.size() - 1);  // our last port is treated a bit different

        std::string cargoFileName = getNextFileForPort(cargoData, portsVisits, portId, dataManager, isLast);
        std::string cargoFilePath = dataManager.cargoFilePath(cargoFileName);
        std::string instructionsOutputPath = dataManager.craneInstructionsOutputPath(portId.getCode(), visitNum);

        algorithm.getInstructionsForCargo(cargoFilePath, instructionsOutputPath);

        Errors cargoErrors;
        Port port(portId, readPortCargoFromFile(cargoFilePath, errors));

        auto optOps = readPackingOperationsFromFile(instructionsOutputPath);  // read the operations to perform, written by the algorithm

        if (!optOps.has_value()) {
            std::cout << "Warning: no packing operations were read" << std::endl;
        } else {
            performPackingOperations(ship, port, *optOps, errors);   // TODO: validate the operations are safe (inside the function call)
            totalNumberOfOps = totalNumberOfOps + optOps.value().size();
        }

        if (!isLast) {
            std::cout << "The ship is continuing to the next port..." << std::endl;
        } else {
            std::cout << "The ship is going into maintenance..." << std::endl;
        }

        printSeparator(1, 1);
    }

    validateNoCargoFilesLeft(cargoData);  // if there are remaining cargo files in the map, we need to print a warning because we couldn't use them

    printSeparator(1, 1);

    std::cout << "The ship has completed its journey. Total number of operations: " << totalNumberOfOps << std::endl;
    results.push_back(intToStr(totalNumberOfOps));  // add number of steps to the table

    printSeparator(1, 5);

    return report;
}
// endregion

// region Simulation core

void
Simulator::performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, Errors &errors) const {// Perform operations on local ship and port

    // TODO: check that any containers that were loaded to the port to unload others, are back in ship

    for (const PackingOperation &op : ops) {

        validatePackingOperation(ship, port, op, errors);

        auto opResult = CranesOperation::preformOperation(op, port, ship);
        if (opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND) {
            std::cerr << "crane received illegal operation, didn't find container with ID: " << op.getContainerId() << std::endl;
            errors.addError("[Algorithm Instruction Error] Didn't find container " + op.getContainerId() + " while at port " + port.getId().getCode() +
                            ", and executing crane operation " +
                            craneOperationToString(op));
        }
        if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP) {
            std::cerr << "Illegal crane operation: " << op << std::endl;
            errors.addError("[Algorithm Instruction Error] Illegal crane operation: " + craneOperationToString(op));
        }
    }

    ship.markCurrentVisitDone();

    // Check if algorithm loaded all required containers (if there is space on the ship)
    bool foundUnloadedContainers = false;
    for (auto portId : ship.getShipRoute().getPorts()) {
        if (portId == port.getId())
            continue;
        if (!port.getContainersForDestination(portId).empty() && !ship.getCargo().isFull()) {
            foundUnloadedContainers = true;
            break;
        }
    }

    if (foundUnloadedContainers)
        errors.addError("[Algorithm Instruction Error] Algorithm didn't load all required containers from port " + port.getId().getCode() +
                        " although ship isn't full");

    std::cout << ops;
    return;
}

void Simulator::initAlgorithm(AbstractAlgorithm &algorithm, const std::string &shipPlanPath,
                              const std::string &shipRoutePath) const {
    std::cout << "Initializing algorithm..." << std::endl;

    algorithm.readShipPlan(shipPlanPath);  // set plan  // TODO: get the return value and document it (if there are errors)
    algorithm.readShipRoute(shipRoutePath);  // set route
    WeightBalanceCalculator algoWeightBalanceCalculator;
    algorithm.setWeightBalanceCalculator(algoWeightBalanceCalculator);

    std::cout << "Success." << std::endl;
    printSeparator(1, 1);
}

ContainerShip Simulator::initSimulation(const std::string &shipPlanPath, const std::string &shipRoutePath, Errors &errors) const {
    std::cout << "Initializing simulation..." << std::endl;

    ShipPlan shipPlan = readShipPlanFromFile(shipPlanPath, errors);
    ShipRoute shipRoute = readShipRouteFromFile(shipRoutePath, errors);
    WeightBalanceCalculator weightBalanceCalculator(shipPlan);

    return ContainerShip(shipPlan, shipRoute, weightBalanceCalculator);
}

void validateLoadOperation(ContainerShip &ship, Port &port, const PackingOperation &op, Errors &errors) {
    const auto &containerId = op.getContainerId();
    if (ship.getCargo().hasContainer(containerId)) {
        errors.addError("[Containers At Port Warning] Duplicate container id (" + op.getContainerId() + ") while loading to ship, ignoring");
    }
    return;
    std::cout << port;
}

void Simulator::validatePackingOperation(ContainerShip &ship, Port &port, const PackingOperation &op, Errors &errors) const {

    switch (op.getType()) {
        case PackingType::load:
            validateLoadOperation(ship, port, op, errors);
            break;
        case PackingType::unload:
        case PackingType::reject:
        case PackingType::move:
            break;
    }
}



// endregion
