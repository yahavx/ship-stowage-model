#include <iostream>
#include "src/io/FileReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/io/ObjectsReader.h"
#include "assert.h"

using namespace std;

void readShipPlanTest();

int main() {
    readShipPlanTest();
}


void readShipPlanTest() {
    ShipPlan shipPlan;
    bool result = readShipPlan("../input-examples/ShipPlan.csv", shipPlan);
    assert(result);
}

void readRouteFileTest() {
    
}