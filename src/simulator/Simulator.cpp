//
// Created by t-yabeny on 4/18/2020.
//

#include "Simulator.h"
#include "../algorithms/stowage/NaiveStowageAlgorithm.h"

std::string getShipPlanPath(const std::string& travel){
    return travel + "/Plan";
}

std::string getShipRoutePath(const std::string& travel){
    return travel + "/Route";
}

void Simulator::runSimulation(IStowageAlgorithm& algorithm, const std::string& travel) {
    ShipPlan shipPlan;
    ShipRoute shipRoute;

    shipPlan = algorithm.readShipPlan(getShipPlanPath(travel));  // TODO: does algorithm should contain the readShipPlan, readShipRoute? or maybe the simulator?
    shipRoute = algorithm.readShipRoute(getShipRoutePath(travel)); //
}
