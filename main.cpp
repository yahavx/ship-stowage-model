#include <iostream>
#include "src/io/FileReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/io/ObjectsReader.h"
#include "assert.h"

using namespace std;

void readShipPlanTest();
void readShipRouteTest();


int main() {
    readShipPlanTest();
}


void readShipPlanTest() {
    ShipPlan shipPlan;
    bool result = readShipPlan("../input-examples/ShipPlan.csv", shipPlan);
    cout << shipPlan;
    assert(result);
}

void readShipRouteTest() {
    ShipRoute shipRoute;
    string path = "../input-examples/RouteFile.csv";
    bool result = readShipRoute(path, shipRoute);
    cout << shipRoute;
    assert(result);
}