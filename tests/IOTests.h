//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_IOTESTS_H
#define SHIP_STOWAGE_MODEL_IOTESTS_H

#include <assert.h>
#include "../src/common/data_objects/ShipPlan.h"
#include "../src/common/io/ObjectsReader.h"
#include "../src/common/utils/Printers.h"
#include "../src/common/utils/ErrorFlags.h"

void inline readShipPlanTest();

void inline readShipRouteTest();

void inline readCargoToPortFromFileTest();

void inline readPackingOperationsTest();

using namespace std;

void inline runIOTests() {
    readShipPlanTest();

    printSeparator(1, 1);
    readShipRouteTest();

//    printSeparator(1, 1);
//    readCargoToPortFromFileTest();

//    printSeparator(1, 1);
//    readPackingOperationsTest();
}

void inline readShipPlanTest() {
    vector<ErrorFlag> errors;
    string path = "../input-examples/test-files/Plan";
    ShipPlan shipPlan = readShipPlanFromFile(path, errors);
    assert(!containsFatalError(errors));

    cout << "Ship plan" << endl;
    cout << shipPlan;
    cout << "Errors:" << endl;
    printErrorsFromFlagVector(errors);
    cout << "Error flag list:" << endl;
    printErrorsFromFlag(errorsVectorToErrorsFlag(errors));
}

void inline readShipRouteTest() {
    vector<ErrorFlag> errors;
    string path = "../input-examples/test-files/Route";
    ShipRoute shipRoute = readShipRouteFromFile(path, errors);
    assert(!containsFatalError(shipRoute));

    cout << "Ship route" << endl;
    cout << shipRoute;
    cout << "Errors list:" << endl;
    printErrorsFromFlagVector(errors);
    cout << "Error flag list:" << endl;
    printErrorsFromFlag(errorsVectorToErrorsFlag(errors));
}

//void inline readCargoToPortFromFileTest() {
//    string path = "../input-examples/test-files/AAAAA_1.cargo_data";
//    optional<ContainerStorage> storage = readPortCargoFromFile(path);
//    assert(storage.has_value());
//    if (storage.has_value()) {
//        cout << endl << "Port after loading cargo: " << endl << *storage;
//    }
//}

//void inline readPackingOperationsTest() {
//    string path = "../input-examples/test-files/BBBBB_2.cargo_data";
//    optional<OPS> operations;
//    cout << "Operations before: " << endl << *operations << endl;
//    operations = readPackingOperationsFromFile(path);
//    assert(operations.has_value());
//    assert(operations->size() == 8);
//    if (operations.has_value()) {
//        cout << "Operations after: " << endl << *operations;
//    }
//}


#endif //SHIP_STOWAGE_MODEL_IOTESTS_H
