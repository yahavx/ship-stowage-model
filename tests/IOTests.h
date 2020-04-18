//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_IOTESTS_H
#define SHIP_STOWAGE_MODEL_IOTESTS_H

#include <assert.h>
#include "../src/data_objects/ShipPlan.h"
#include "../src/common/io/ObjectsReader.h"
#include "../src/utils/Printers.h"


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
    string path = "../input-examples/tests/Plan";
    optional<ShipPlan> shipPlan = readShipPlanFromFile(path);
    assert(result);
    if (shipPlan.has_value()) {
        cout << *shipPlan;
    }
}

void inline readShipRouteTest() {
    string path = "../input-examples/tests/Route";
    optional<ShipRoute> shipRoute = readShipRouteFromFile(path);
    assert(shipRoute.has_value());
    assert(shipRoute.getPorts().size() == 4);
    if (shipRoute.has_value()) {
        cout << *shipRoute;
    }
}

void inline readCargoToPortFromFileTest() {
    string path = "../input-examples/tests/AGHCS_17.cargo_data";
    optional<Port> port = readCargoToPortFromFile(path);
    assert(port.has_value());
    if (port.has_value()) {
        cout << endl << "Port after loading cargo: " << endl << *port;
    }
}

void inline readPackingOperationsTest() {

    string path = "../input-examples/tests/DASDF_13.cargo_data";
    optional<OPS> operations;
    cout << "Operations before: " << endl << *operations << endl;
    operations = readPackingOperationsFromFile(path);
    assert(result);
    assert(operations.size() == 8);
    cout << "Operations after: " << endl << *operations;
}


#endif //SHIP_STOWAGE_MODEL_IOTESTS_H
