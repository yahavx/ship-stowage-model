//
// Created by t-yabeny on 4/18/2020.
//

#include "Simulator.h"
#include "../algorithms/stowage/NaiveStowageAlgorithm.h"
#include "../common/io/ObjectsReader.h"
#include "../utils/Printers.h"

std::string getShipPlanPath(const std::string& travel){
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string& travel){
    return travel + "/Route";
}

void Simulator::runSimulation(IStowageAlgorithm &algorithm, const std::string& travel) {
    // Get plan and route paths
    std::string shipPlanPath = getShipPlanPath(travel);
    std::string shipRoutePath = getShipRoutePath(travel);

    // Init for simulation
    std::optional<ShipPlan> optShipPlan = readShipPlanFromFile(shipPlanPath);
    std::optional<ShipRoute> optShipRoute = readShipRouteFromFile(shipRoutePath);

    if (!optShipPlan.has_value() || !optShipRoute.has_value()) { // TODO: handle error (maybe its okay like this)
        std::cout << "Simulation failed: couldn't initialize from files" << std::endl;
        return;
    }

    ShipPlan &shipPlan = *optShipPlan;
    ShipRoute &shipRoute = *optShipRoute;

    WeightBalanceCalculator weightBalanceCalculator(shipPlan);

    ContainerShip ship(shipPlan, shipRoute, weightBalanceCalculator);  // TODO: add balance calculator

    // Init for algorithm
    algorithm.setShipPlanFromPath(shipPlanPath);
    algorithm.setShipRouteFromPath(shipRoutePath);

    algorithm.setWeightBalanceCalculator(weightBalanceCalculator);

    std::string input = "../input-examples/Travel_3/AAAAA_17.cargo_data", output = "../input-examples/results";  // Until one works fine..
    algorithm.getInstructionsForCargo(input, output);
    
    std::optional<OPS> optionalOps = readPackingOperationsFromFile(output);

    if (!optionalOps.has_value()) {
        std::cout << "No operations found.";
    }
    else {
        std::cout << "Operations generated:" << std::endl;
        std::cout << *optionalOps;
    }
}
