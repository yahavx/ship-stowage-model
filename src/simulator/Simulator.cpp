//
// Created by t-yabeny on 4/18/2020.
//

#include "Simulator.h"
#include "../algorithms/stowage/NaiveStowageAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../utils/Printers.h"
#include "SimulatorUtil.h"
#include "../algorithms/CranesOperation.h"
#include "../common/io/FileReader.h"

// region Constructors

Simulator::Simulator() {
    staticOutputFile = "../simulation-output/cargo_instructions";
}
// endregion

// region Simulation core

std::string getShipPlanPath(const std::string &travel) {
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string &travel) {
    return travel + "/Route";
}

void Simulator::runSimulation(IStowageAlgorithm &algorithm, const std::string &travel) {
    // Validate root folder exists
    if (!isDirectoryExists(travel)) {
        std::cout << "Simulation failed: the travel path supplied is not a directory" << std::endl;
        return;
    }

    ///////////////////////
    ///     Init         //
    ///////////////////////

    // Get plan and route paths
    std::string shipPlanPath = getShipPlanPath(travel);
    std::string shipRoutePath = getShipRoutePath(travel);

    // Init for simulation
    ContainerShip ship;
    bool res = initSimulation(shipPlanPath, shipRoutePath, ship);
    if (!res)
        return;  // failed to init simulation, errors were printed inside

    // Init for algorithm
    initAlgorithm(algorithm, shipPlanPath, shipRoutePath);

    StringToStringVectorMap map = sortTravelCargoData(travel);  // get list of .cargo_data files, ordered for each port

    std::cout << "Validating route..." << std::endl;
    filterUnusedPorts(map, ship.getShipRoute());  // remove the port files which are not on the ship route

    printSeparator(1, 1);

    ///////////////////////
    /// Start simulation //
    ///////////////////////

    std::cout << "The ship has started its journey!" << std::endl;

    printSeparator(1, 1);

    for (const PortId &portId : ship.getShipRoute().getPorts()) {  // Start the journey
        std::cout << "The ship has docked at port " << portId.getCode() << "." << std::endl;

        Port port(portId);

        res = getInstructionsForCargo(algorithm, travel, map,
                                      port);  // triggers algorithm getInstructions(), sets port ContainerStorage if needed

        if (!res)
            continue; // failed to read current dock file, errors were printed inside  // TODO: check if we can continue anyways

        auto optOps = readPackingOperationsFromFile(staticOutputFile);  // read the operations to perform, written by the algorithm

        if (!optOps.has_value()) {
            std::cout << "Warning: no packing operations were read" << std::endl;
        } else {
            performPackingOperations(ship, port, *optOps);
        }

        std::cout << "The ship is continuing to the next port..." << std::endl;

        printSeparator(1, 1);
    }

    printSeparator(1, 1);

    std::cout << "The ship has completed its journey!" << std::endl;

    printSeparator(1, 1);
}

void Simulator::performPackingOperations(ContainerShip &ship, Port &port, const OPS &ops) const {// Perform operations on local ship and port
    for (const PackingOperation &op : ops) {
        auto opResult = CranesOperation::preformOperation(op, port, ship);
        if (opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND)
            std::cout << "Crane received illegal operation, didn't find container with ID:"
                      << op.getContainerId()
                      << std::endl;
        if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP)
            std::cout << "Crane received illegal operation: " << op << std::endl;
    }

    std::cout << ops;
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
                               ContainerShip &ship) const {
    std::cout << "Initializing simulation..." << std::endl;
    std::optional<ShipPlan> optShipPlan = readShipPlanFromFile(shipPlanPath);
    std::optional<ShipRoute> optShipRoute = readShipRouteFromFile(shipRoutePath);

    if (!optShipPlan.has_value() || !optShipRoute.has_value()) { // TODO: handle error (maybe its okay like this)
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
}
// endregion

void test(IStowageAlgorithm &algorithm) {  // TODO: remove

    std::string input = "../input-examples/Travel_3/AAAAA_17.cargo_data", output = "../input-examples/results";  // Until one works fine..
    algorithm.getInstructionsForCargo(input, output);

    std::optional<OPS> optionalOps = readPackingOperationsFromFile(output);

    if (!optionalOps.has_value()) {
        std::cout << "No operations found.";
    } else {
        std::cout << "Operations generated:" << std::endl;
        std::cout << *optionalOps;
    }
}