//
// Created by t-yabeny on 4/18/2020.
//

#include "Simulator.h"
#include "../algorithms/stowage/NaiveStowageAlgorithm.h"
#include "../common/io/ObjectsReader.h"

std::string getShipPlanPath(const std::string& travel){
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string& travel){
    return travel + "/Route";
}

void Simulator::runSimulation(IStowageAlgorithm& algorithm, const std::string& travel) {
    ShipPlan shipPlan;
    ShipRoute shipRoute;

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

    // Init for algorithm
    algorithm.setShipPlanFromPath(shipPlanPath);
    algorithm.setShipRouteFromPath(shipRoutePath);

}
