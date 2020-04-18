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
    readShipPlanFromFile(shipPlanPath, shipPlan);
    readShipRouteFromFile(shipRoutePath, shipRoute);

    // Init for algorithm
    algorithm.setShipPlanFromPath(shipPlanPath);
    algorithm.setShipRouteFromPath(shipRoutePath);

}
