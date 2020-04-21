//
// Created by t-yabeny on 4/18/2020.
//

#include "Simulator.h"
#include "SimulatorUtil.h"
#include "../algorithms/stowage/NaiveStowageAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../utils/Printers.h"
#include "../algorithms/CranesOperation.h"
#include "../common/io/FileReader.h"
#include "../utils/UtilFunctions.h"

// region Constructors

Simulator::Simulator() {
    staticOutputFile = "../simulation-output/cargo_instructions";
    NaiveStowageAlgorithm *naiveStowageAlgorithm = new NaiveStowageAlgorithm();
    NaiveStowageAlgorithm *naiveStowageAlgorithm2 = new NaiveStowageAlgorithm();
    algorithms.push_back(naiveStowageAlgorithm);
    algorithms.push_back(naiveStowageAlgorithm2);
}
// endregion

// region Simulation run

std::string getShipPlanPath(const std::string &travel);

std::string getShipRoutePath(const std::string &travel);

void Simulator::runSimulations(StringVector travels, const std::string &outputDir) {
    StringStringVector results;  // table for results
    StringStringVector errors;  // table for errors

    initSimulationTables(results, errors, travels, algorithms);  // add columns names and set table structure

    for (longUInt i = 0; i < algorithms.size(); i ++) {
        auto &algorithm = algorithms[i];

        for (auto &travel: travels) {
            printSimulationInfo(travel, algorithm);  // prints starting messages

            StringStringVector simulationResults = runSimulation(*algorithm, travel);  // run simulation, collect data

            addTravelResultsToTable(simulationResults, results, errors, i + 1);  // save collected data from the travel to the tables
        }
    }

    // add all missing data that can now be collected (sums, errors)
    orderSimulationTables(results, errors);

    // save
    saveSimulationTables(results, errors, outputDir);
}

StringStringVector Simulator::runSimulation(IStowageAlgorithm &algorithm, const std::string &travel) {
    StringStringVector report(2, StringVector());  // 2 rows, first is a single entry (number of steps), second is zero or more entries (number of errors)
    StringVector &results = report[0];
    StringVector &errors = report[1];

    // validate root folder exists
    if (!isDirectoryExists(travel)) {
        std::cerr << "Simulation failed: the travel path supplied is not a directory" << std::endl;
        results.push_back("<SimulationFailed>");
        return report;  // we don't add an error because its not the algorithm fault
    }

    ////////////////////////
    ///     Init         ///
    ////////////////////////

    // Get plan and route paths
    std::string shipPlanPath = getShipPlanPath(travel);
    std::string shipRoutePath = getShipRoutePath(travel);

    // Init for simulation
    ContainerShip ship;
    bool res = initSimulation(shipPlanPath, shipRoutePath, ship, errors);
    if (!res) {
        results.push_back("<SimulationFailed>");
        return report;  // failed to init simulation
    }

    // Init for algorithm
    initAlgorithm(algorithm, shipPlanPath, shipRoutePath);

    StringToStringVectorMap map = sortTravelCargoData(travel);  // get list of .cargo_data files, ordered for each port

    std::cout << "Validating route..." << std::endl;
    filterUnusedPorts(map, ship.getShipRoute());  // remove the port files which are not on the ship route
    std::cout << "Finished." << std::endl;

    printSeparator(1, 1);

    ////////////////////////
    /// Start simulation ///
    ////////////////////////

    std::cout << "The ship has started its journey!" << std::endl;

    printSeparator(1, 1);

    auto &ports = ship.getShipRoute().getPorts();
    int totalNumberOfOps = 0;
    for (longUInt i = 0; i < ports.size(); i++) {  // Start the journey
        auto &portId = ports[i];

        std::cout << "The ship has docked at port " << portId.getCode() << "." << std::endl;

        Port port(portId);

        bool isLast = (i == ports.size() - 1);  // our last port is treated a bit different

        res = getInstructionsForCargo(algorithm, travel, map, port, isLast);
        // triggers algorithm getInstructions(), sets port ContainerStorage if needed

        if (!res)
            continue; // failed to read current dock file, errors were printed inside

        auto optOps = readPackingOperationsFromFile(staticOutputFile);  // read the operations to perform, written by the algorithm

        if (!optOps.has_value()) {
            std::cout << "Warning: no packing operations were read" << std::endl;
        } else {
            performPackingOperations(ship, port, *optOps, errors); // TODO: validate the operations are safe (inside the function call)
            totalNumberOfOps = totalNumberOfOps + optOps.value().size();
        }

        if (!isLast) {
            std::cout << "The ship is continuing to the next port..." << std::endl;
        } else {
            std::cout << "The ship is going into maintenance..." << std::endl;
        }

        printSeparator(1, 1);
    }

    validateNoCargoFilesLeft(map);  // if there are remaining cargo files in the map, we need to print a warning because we couldn't use them

    printSeparator(1, 1);

    std::cout << "The ship has completed its journey. Total number of operations: " << totalNumberOfOps << std::endl;
    results.push_back(intToString(totalNumberOfOps));  // add number of steps to the table

    printSeparator(1, 5);

    return report;
}

// endregion

// region Simulation core

std::string getShipPlanPath(const std::string &travel) {
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string &travel) {
    return travel + "/Route";
}

void
Simulator::performPackingOperations(ContainerShip &ship, Port &port, const OPS &ops, StringVector &errors) const {// Perform operations on local ship and port

    // TODO: when loading from port, check that container exists on port, and that x,y,z is a legal position
    // TODO: when loading from port, check that same container ID is not in the ship
    // TODO: check that when leaving the port, all containers are loaded (if possible)
    // TODO: check that any containers that were loaded to the port to unload others, are back in ship



    for (const PackingOperation &op : ops) {


        auto opResult = CranesOperation::preformOperation(op, port, ship);
        if (opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND)
            std::cerr << "Crane received illegal operation, didn't find container with ID:" << op.getContainerId() << std::endl;
        if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP)
            std::cerr << "Crane received illegal operation: " << op << std::endl;
    }

    std::cout << ops;
    return;
    std::cout << errors;
}

void Simulator::initAlgorithm(IStowageAlgorithm &algorithm, const std::string &shipPlanPath,
                              const std::string &shipRoutePath) const {
    std::cout << "Initializing algorithm..." << std::endl;

    algorithm.setShipPlanFromPath(shipPlanPath);  // set plan
    algorithm.setShipRouteFromPath(shipRoutePath);  // set route
    WeightBalanceCalculator algoWeightBalanceCalculator;
    algorithm.setWeightBalanceCalculator(algoWeightBalanceCalculator);

    std::cout << "Success." << std::endl;
    printSeparator(1, 1);
}

bool Simulator::initSimulation(const std::string &shipPlanPath, const std::string &shipRoutePath,
                               ContainerShip &ship, StringVector &errors) const {
    std::cout << "Initializing simulation..." << std::endl;
    std::optional<ShipPlan> optShipPlan = readShipPlanFromFile(shipPlanPath);
    std::optional<ShipRoute> optShipRoute = readShipRouteFromFile(shipRoutePath);

    if (!optShipPlan.has_value() || !optShipRoute.has_value()) {
        std::cout << "Simulation failed: couldn't initialize from files" << std::endl;
        return false;
    }

    ShipPlan &shipPlan = *optShipPlan;
    ShipRoute &shipRoute = *optShipRoute;

    WeightBalanceCalculator weightBalanceCalculator(shipPlan);
    ship = ContainerShip(shipPlan, shipRoute, weightBalanceCalculator);

    std::cout << "Success." << std::endl;
    printSeparator(1, 1);
    return true;
    std::cout << errors;  // TODO
    return true;
}

// endregion
