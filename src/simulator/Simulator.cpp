//
// Created by t-yabeny on 4/18/2020.
//

#include "Simulator.h"
#include "../algorithms/stowage/NaiveStowageAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../utils/Printers.h"
#include "SimulatorUtil.h"

// region Simulation core

std::string getShipPlanPath(const std::string &travel) {
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string &travel) {
    return travel + "/Route";
}

std::string getCargoPath(const std::string &travel, const std::string &cargoFile) {
    return travel + "/" + cargoFile;
}

void test(IStowageAlgorithm &algorithm);

void Simulator::runSimulation(IStowageAlgorithm &algorithm, const std::string &travel) {
    // Get plan and route paths
    std::string shipPlanPath = getShipPlanPath(travel);
    std::string shipRoutePath = getShipRoutePath(travel);

    // Init for simulation
    std::cout << "***Initializing simulation***" << std::endl;
    std::optional<ShipPlan> optShipPlan = readShipPlanFromFile(shipPlanPath);
    std::optional<ShipRoute> optShipRoute = readShipRouteFromFile(shipRoutePath);

    if (!optShipPlan.has_value() || !optShipRoute.has_value()) { // TODO: handle error (maybe its okay like this)
        std::cout << "Simulation failed: couldn't initialize from files" << std::endl;
        return;
    }

    printSeparator(1, 1);

    ShipPlan &shipPlan = *optShipPlan;
    ShipRoute &shipRoute = *optShipRoute;
    WeightBalanceCalculator weightBalanceCalculator(shipPlan);
    ContainerShip ship(shipPlan, shipRoute, weightBalanceCalculator);

    // Init for algorithm
    std::cout << "***Initializing algorithm***" << std::endl;
    algorithm.setShipPlanFromPath(shipPlanPath);
    algorithm.setShipRouteFromPath(shipRoutePath);
    WeightBalanceCalculator algoWeightBalanceCalculator(shipPlan);
    algorithm.setWeightBalanceCalculator(algoWeightBalanceCalculator);

    printSeparator(1, 1);

    // Start simulation
    StringToStringVectorMap map = sortTravelCargoData(travel);  // get list of .cargo_data files, ordered for each port
    std::cout << map;
//    test(algorithm);  // remove comment to test one .cargo_data
//    return;

    std::cout << "Validating route..." << std::endl;
    filterUnusedPorts(map, shipRoute);  // remove the port files which are not on the ship route
//    filterTwiceInARowPorts(shipRoute);  // this is not needed probably, will remove it when i'm sure

    printSeparator(1, 1);

    std::cout << "The ship has started its journey!" << std::endl;

    printSeparator(1,1);

    for (const PortId &portId : shipRoute.getPorts()) {  // Start the journey
        std::cout << "The ship has docked at port " << portId.getCode() << "." << std::endl;

        std::optional<std::string> cargoFile = getNextFileForPort(map, portId.getCode());  // TODO: check if we need to save storage that wasn't loaded to the ship from the port

        if (!cargoFile.has_value()) {
            std::cout << "Warning: no cargo file for current visit, ship will only unload" << std::endl;
            algorithm.getInstructionsForCargo(unloadOnly + portId.getCode(), staticOutputFile);  // TODO: fix
        }

        else {
            algorithm.getInstructionsForCargo(*cargoFile, staticOutputFile);
        }

        auto optOps = readPackingOperationsFromFile(staticOutputFile);

        if (!optOps.has_value()) {
            std::cout << " Warning: no packing operations were read" << std::endl;
        }

        else {
            // TODO: perform operations

        }

        std::cout << "The ship is continuing to the next port..." << std::endl;

        printSeparator(0,0);
    }

    printSeparator(1,1 );

    std::cout << "The ship has completed its journey!" << std::endl;

    printSeparator(1,1);





}
// endregion

void test(IStowageAlgorithm &algorithm) {

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