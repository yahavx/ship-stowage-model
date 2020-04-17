#include <iostream>
#include "src/io/FileReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/io/ObjectsReader.h"
#include <cassert>
#include "src/actors/Port.h"

using namespace std;

void readShipPlanTest();
void readShipRouteTest();
void readCargoToPortFromFileTest();


int main() {
    readCargoToPortFromFileTest();
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

void readCargoToPortFromFileTest() {
    Port port(PortId("DLDAS"));
    string path = "../input-examples/PortCargo.cargo_data";
    std::cout << "Port before loading cargo: " << endl << port << endl;
    bool result = readCargoToPortFromFile(path, port);

    std::cout << endl << "Port after loading cargo: " << endl << port;
}
