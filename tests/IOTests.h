//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_IOTESTS_H
#define SHIP_STOWAGE_MODEL_IOTESTS_H


//#undef NDEBUG

#include <assert.h>
#include "../src/common/data_objects/ShipPlan.h"
#include "../src/common/io/ObjectsReader.h"
#include "../src/common/utils/Printers.h"
#include "../src/common/utils/Errors.h"

void inline readShipPlanTest();

void inline readShipRouteTest();

void inline readFatalShipPlan();

void inline readFatalShipRoute();

void inline readCargoToPortFromFileTest();

void inline readPackingOperationsTest();

using namespace std;

void inline runIOTests() {
    readShipPlanTest();

    printSeparator(1, 1);
    readFatalShipPlan();

    printSeparator(1, 1);
    readShipRouteTest();

    printSeparator(1, 1);
    readFatalShipRoute();

    printSeparator(1, 1);
    readCargoToPortFromFileTest();

    printSeparator(1, 1);
    cout << "Tests finished successfully";
//    printSeparator(1, 1);
//    readCargoToPortFromFileTest();

//    printSeparator(1, 1);
//    readPackingOperationsTest();
}

void inline readShipPlanTest() {
    cout << "readShipPlanTest()" << endl << endl;
    Errors errors;
    string path = "../input-examples/test-files/Plan";
    ShipPlan shipPlan = readShipPlanFromFile(path, errors);
    assert(!errors.hasFatalError());

    cout << "Ship plan" << endl;
    cout << shipPlan;
    cout << errors;
    printErrorsFromFlag(errors.toErrorFlag());
}

void inline readFatalShipPlan() {
    cout << "readFatalShipPlan()" << endl << endl;

    StringVector paths = {"../input-examples/test-files/FatalPlan", "../input-examples/test-files/FatalPlan2", "../input-examples/test-files/FatalPlan_NoFile"};

    for (auto &path: paths) {
        Errors errors;
        cout << extractFilenameFromPath(path, false) << endl;
        ShipPlan shipPlan = readShipPlanFromFile(path, errors);
        cout << errors;
        assert(!errors.hasFatalError());
        printSeparator(0, 0);
    }
}

void inline readShipRouteTest() {
    cout << "readShipRouteTest()" << endl << endl;
    Errors errors;

    string path = "../input-examples/test-files/Route";

    ShipRoute shipRoute = readShipRouteFromFile(path, errors);
    assert(!errors.hasFatalError());

    cout << "Ship route" << endl;
    cout << shipRoute;
    cout << errors;
    printErrorsFromFlag(errors.toErrorFlag());
}

void inline readFatalShipRoute() {
    cout << "readFatalShipRoute()" << endl << endl;
    StringVector paths = {"../input-examples/test-files/FatalRoute", "../input-examples/test-files/FatalRoute2",
                          "../input-examples/test-files/FatalRoute_NoFile"};

    for (auto &path: paths) {
        Errors errors;
        cout << extractFilenameFromPath(path) << endl;
        ShipRoute shipRoute = readShipRouteFromFile(path, errors);
        cout << errors;
        assert(errors.hasFatalError());
        printSeparator(0,0);
    }
}


void inline readCargoToPortFromFileTest() {
    cout << "readCargoToPortFromFileTest()" << endl << endl;

    StringVector paths = {"../input-examples/test-files/AAAAA_1.cargo_data", "../input-examples/test-files/NOFIL_23.cargo_data"};

    for (auto &path: paths) {
        Errors errors;
        cout << extractFilenameFromPath(path) << endl;
        ContainerStorage storage = readPortCargoFromFile(path, errors);
        cout << "Container Storage:" << endl;
        cout << storage;
        cout << errors;
        printErrorsFromFlag(errors.toErrorFlag());
        printSeparator(0,0);
    }
}

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
