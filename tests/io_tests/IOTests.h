//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_IOTESTS_H
#define SHIP_STOWAGE_MODEL_IOTESTS_H

#include <assert.h>
#include "../../src/data_objects/ShipPlan.h"
#include "../../src/common/io/ObjectsReader.h"

void inline readShipPlanTest();
void inline readShipRouteTest();
void inline readCargoToPortFromFileTest();
void inline readPackingOperationsTest();

using namespace std;

void inline runIOTests() {
    readShipPlanTest();
    printSeparator();

    readShipRouteTest();
    printSeparator();

    readCargoToPortFromFileTest();
    printSeparator();

    readPackingOperationsTest();
}

void inline readShipPlanTest() {
    ShipPlan shipPlan;
    bool result = readShipPlanFromFile("../input-examples/tests/ShipPlan.csv", shipPlan);
    assert(result);
    cout << shipPlan;
}

void inline readShipRouteTest() {
    ShipRoute shipRoute;
    string path = "../input-examples/tests/RouteFile.csv";
    bool result = readShipRouteFromFile(path, shipRoute);
    assert(result);
    assert(shipRoute.getPorts().size() == 4);
    cout << shipRoute;
}

void inline readCargoToPortFromFileTest() {
    string path = "../input-examples/tests/AGHCS_17.cargo_data";
    optional<Port> optPort = readCargoToPortFromFile(path);
    assert(optPort.has_value());
    if (optPort.has_value()) {
        cout << endl << "Port after loading cargo: " << endl << *optPort;
    }
}

void inline readPackingOperationsTest() {
    OPS operations;
    string path = "../input-examples/tests/CargoInstructions";
    cout << "Operations before: " << endl << operations << endl;
    bool result = readPackingOperationsFromFile(path, operations);
    assert(result);
    assert(operations.size() == 8);
    cout << "Operations after: " << endl << operations;
}


#endif //SHIP_STOWAGE_MODEL_IOTESTS_H
