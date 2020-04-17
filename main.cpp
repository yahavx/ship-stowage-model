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
    cout << endl;
    readShipRouteTest();
}


void readShipPlanTest() {
    ShipPlan shipPlan;
    bool result = readShipPlanFromFile("../input-examples/ShipPlan.csv", shipPlan);
    assert(result);
    cout << shipPlan;
}

void readShipRouteTest() {
    ShipRoute shipRoute;
    string path = "../input-examples/RouteFile.csv";
    bool result = readShipRouteFromFile(path, shipRoute);
    assert(result);
    cout << shipRoute;
}
