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
#include "../common/utils/ErrorFlags.h"
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

Simulator::Simulator(const std::string &outputDir) : outputDir(outputDir) {
    NaiveStowageAlgorithm *naiveStowageAlgorithm = new NaiveStowageAlgorithm();
//    NaiveStowageAlgorithm *naiveStowageAlgorithm2 = new NaiveStowageAlgorithm();
    BadAlgorithm *badAlgorithm = new BadAlgorithm();

    algorithms.push_back(naiveStowageAlgorithm);
//    algorithms.push_back(naiveStowageAlgorithm2);
    algorithms.push_back(badAlgorithm);
}
// endregion

// region Simulation run

void Simulator::runSimulations(const std::string &travelPath) {
    StringStringVector results;  // table for results
    ErrorVector generalErrors;

    initOutputFolders(outputDir, generalErrors);
    StringVector travels = collectTravels(travelPath, generalErrors);

    if (!generalErrors.empty()) {  // any init error is fatal so we have to terminate
        saveErrorFile(outputDir, s_generalErrorsTableName, generalErrors);
        cleanOutputFolders(outputDir, generalErrors);
        return;
    }

    initResultsTable(results, travels, algorithms);  // add columns names and set table structure
    for (auto &travel: travels) {

        if (!isTravelValid(travel, generalErrors)) {
            continue;
        }

        for (longUInt i = 0; i < algorithms.size(); i++) {
            auto &algorithm = algorithms[i];

            printSimulationInfo(travel, algorithm);  // prints starting messages

            auto craneOutputDir = getCraneInstructionsSimulationFolder(outputDir, intToString(i), extractFilenameFromPath(travel,
                                                                                                                          false));  // crane instructions directory for the current run  // TODO: change i to algorithm name
            createFolder(craneOutputDir);
            StringStringVector simulationResults = runSimulation(*algorithm, travel, craneOutputDir);  // run simulation, collect data

            addSimulationResultToTable(simulationResults, results, i + 1);  // save collected data from the travel to the tables
        }
    }

    finalizeResultsTable(results);

    saveSimulationTables(outputDir, results, generalErrors);

    cleanOutputFolders(outputDir);  // remove temp and errors (if empty), can disable it to debug
}

StringStringVector Simulator::runSimulation(AbstractAlgorithm &algorithm, const std::string &travel, const std::string &craneOutputDir) {
    StringStringVector report = StringStringVector(2, StringVector());
    StringVector &results = report[0];
    StringVector &errors = report[1];  // TODO: remove error or errorFlags (keep one)
    std::vector<ErrorFlag> errorFlags;

    // the travel is validated before the simulation starts, but we do this for extra safety
    if (!isTravelValid(travel)) {
        results.push_back("-2");  // it shouldn't happen
        return report;
    }

    ////////////////////////
    ///     Init         ///
    ////////////////////////

    // Get plan and route paths
    std::string shipPlanPath = getShipPlanPath(travel);
    std::string shipRoutePath = getShipRoutePath(travel);

    // Init for simulation
    ContainerShip ship = initSimulation(shipPlanPath, shipRoutePath, errorFlags);

    // Init for algorithm
    initAlgorithm(algorithm, shipPlanPath, shipRoutePath);

    StringToStringVectorMap cargoData = sortTravelCargoData(travel);  // get list of .cargo_data files, ordered for each port
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

        std::string cargoFileName = getNextFileForPort(cargoData, portsVisits, portId, outputDir, isLast);
        std::string cargoFilePath = getCargoPath(travel, cargoFileName);
        std::string instructionsOutputPath = getCraneInstructionsOutputFilePath(craneOutputDir, portId, visitNum);

        algorithm.getInstructionsForCargo(cargoFilePath, instructionsOutputPath);

        std::vector<ErrorFlag> cargoErrors;
        Port port(portId, readPortCargoFromFile(cargoFilePath, cargoErrors));

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
    results.push_back(intToString(totalNumberOfOps));  // add number of steps to the table

    printSeparator(1, 5);

    return report;
}
// endregion

// region Simulation core

void
Simulator::performPackingOperations(ContainerShip &ship, Port &port, const Operations &ops, StringVector &errors) const {// Perform operations on local ship and port

    // TODO: check that any containers that were loaded to the port to unload others, are back in ship

    for (const PackingOperation &op : ops) {

        validatePackingOperation(ship, port, op, errors);

        auto opResult = CranesOperation::preformOperation(op, port, ship);
        if (opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND) {
            std::cerr << "Crane received illegal operation, didn't find container with ID:" << op.getContainerId() << std::endl;
            errors.push_back("didn't find container: " + op.getContainerId() + " while at port: " + port.getId().getCode() + ", executing crane operation: " +
                             craneOperationToString(op));
        }
        if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP) {
            std::cerr << "Illegal crane operation: " << op << std::endl;
            errors.push_back("Illegal crane operation: " + craneOperationToString(op));
        }
    }

    ship.markCurrentVisitDone();

    // Check if algorithm loaded all required containers if there is space on the ship
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
        errors.push_back("Algorithm didn't load all required containers from port " + port.getId().getCode() + " although ship isn't full");

    std::cout << ops;
    return;
    std::cout << errors;
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

ContainerShip Simulator::initSimulation(const std::string &shipPlanPath, const std::string &shipRoutePath, std::vector<ErrorFlag> &errors) const {
    std::cout << "Initializing simulation..." << std::endl;

    ShipPlan shipPlan = readShipPlanFromFile(shipPlanPath, errors);
    ShipRoute shipRoute = readShipRouteFromFile(shipRoutePath, errors);
    WeightBalanceCalculator weightBalanceCalculator(shipPlan);

    return ContainerShip(shipPlan, shipRoute, weightBalanceCalculator);
}

void validateLoadOperation(ContainerShip &ship, Port &port, const PackingOperation &op, StringVector &errors) {
    const auto &containerId = op.getContainerId();
    if (ship.getCargo().hasContainer(containerId)) {
        errors.push_back("Duplicate container id: " + op.getContainerId() + " while loading to ship");
    }
    return;
    std::cout << port;
}

void Simulator::validatePackingOperation(ContainerShip &ship, Port &port, const PackingOperation &op, StringVector &errors) const {

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
