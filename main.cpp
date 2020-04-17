#include <iostream>
#include "src/io/FileReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/io/ObjectsReader.h"
#include <cassert>

using namespace std;

void readShipPlanTest();
void readShipRouteTest();


int main() {
    readShipPlanTest();
    cout << endl << endl;
    readShipRouteTest();
}


void readShipPlanTest() {
    cout << "Attempting to read ship plan:" << endl;
    ShipPlan shipPlan;
    bool result = readShipPlan("../input-examples/ShipPlan.csv", shipPlan);
    cout << shipPlan;
    assert(result);
}

void readShipRouteTest() {
    cout << "Attempting to read ship route:" << endl;
    ShipRoute shipRoute;
    string path = "../input-examples/RouteFile.csv";
    bool result = readShipRoute(path, shipRoute);
    cout << shipRoute;
    assert(result);
}
