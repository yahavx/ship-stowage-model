//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_IOTESTS_H
#define SHIP_STOWAGE_MODEL_IOTESTS_H

#include <cassert>
#include "../../src/data_objects/ShipPlan.h"
#include "../../src/io/ObjectsReader.h"


void inline readShipPlanTest() {
    ShipPlan shipPlan;
    bool result = readShipPlanFromFile("../input-examples/tests/ShipPlan.csv", shipPlan);
    assert(result);
    std::cout << shipPlan;
}

void inline readShipRouteTest() {
    ShipRoute shipRoute;
    std::string path = "../input-examples/tests/RouteFile.csv";
    bool result = readShipRouteFromFile(path, shipRoute);
    assert(result);
    assert(shipRoute.getPorts().size() == 4);
    std::cout << shipRoute;
}

void inline readCargoToPortFromFileTest() {
    Port port(PortId("DLDAS"));
    std::string path = "../input-examples/tests/PortCargo.cargo_data";
    std::cout << "Port before loading cargo: " << std::endl << port << std::endl;
    bool result = readCargoToPortFromFile(path, port);
    assert(result);
    std::cout << std::endl << "Port after loading cargo: " << std::endl << port;
}


#endif //SHIP_STOWAGE_MODEL_IOTESTS_H
