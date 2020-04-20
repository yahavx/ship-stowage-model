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
    NaiveStowageAlgorithm* naiveStowageAlgorithm = new NaiveStowageAlgorithm();
    algorithms.push_back(naiveStowageAlgorithm);  // TODO: passing by value kills the build so I think we can only pass a pointer
                                                  // TODO: if we keep it like this, may need a destructor (default can be fine also maybe)
}
// endregion

// region Simulation run

std::string getShipPlanPath(const std::string &travel);

std::string getShipRoutePath(const std::string &travel);

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
    std::cout << "Finished." << std::endl;

    printSeparator(1, 1);

    ///////////////////////
    /// Start simulation //
    ///////////////////////

    std::cout << "The ship has started its journey!" << std::endl;

    printSeparator(1, 1);

    auto &ports = ship.getShipRoute().getPorts();

    for (longUInt i = 0; i < ports.size(); i++) {  // Start the journey
        auto &portId = ports[i];

        std::cout << "The ship has docked at port " << portId.getCode() << "." << std::endl;

        Port port(portId);

        bool isLast = (i == ports.size() - 1);  // our last port is treated a bit different

        res = getInstructionsForCargo(algorithm, travel, map, port, isLast);
        // triggers algorithm getInstructions(), sets port ContainerStorage if needed

        if (!res)
            continue; // failed to read current dock file, errors were printed inside  // TODO: check if we can continue anyways

        auto optOps = readPackingOperationsFromFile(staticOutputFile);  // read the operations to perform, written by the algorithm

        if (!optOps.has_value()) {
            std::cout << "Warning: no packing operations were read" << std::endl;
        } else {
            performPackingOperations(ship, port, *optOps);  // TODO: validate the operations are safe (inside the function call)
        }

        if (!isLast) {
            std::cout << "The ship is continuing to the next port..." << std::endl;
        } else {
            std::cout << "The ship finished its work in the final destination..." << std::endl;
        }

        printSeparator(1, 1);
    }

    validateNoCargoFilesLeft(
            map);  // if there are remaining cargo files in the map, we need to print a warning because we couldn't use them

    printSeparator(1, 1);

    std::cout << "The ship has completed its journey!" << std::endl;

    printSeparator(1, 1);
}

void Simulator::runSimulations(StringVector travels) {
    for (auto& algorithm : algorithms) {
        for (auto& travel: travels) {  // TODO: reset the algorithm after every travel
            runSimulation(*algorithm, travel);  // TODO: save results to a file
        }
    }

}
// endregion

// region Simulation core

std::string getShipPlanPath(const std::string &travel) {
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string &travel) {
    return travel + "/Route";
}

void Simulator::performPackingOperations(ContainerShip &ship, Port &port,
                                         const OPS &ops) const {// Perform operations on local ship and port
    for (const PackingOperation &op : ops) {  // TODO: validate the operations are safe (are we already doing this?)
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
