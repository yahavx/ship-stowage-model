#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/common/io/ObjectsReader.h"
#include <cassert>
#include <fstream>
#include "src/actors/Port.h"
#include "src/utils/Printers.h"
#include "tests/IOTests.h"
#include "src/simulator/Simulator.h"
#include "src/algorithms/stowage/NaiveStowageAlgorithm.h"

using namespace std;

int main() {
    Simulator simulator;

    NaiveStowageAlgorithm naiveStowageAlgorithm;
    IStowageAlgorithm &algorithm(naiveStowageAlgorithm);

    string path = "../input-examples/tests";

    simulator.runSimulation(algorithm, path);
}