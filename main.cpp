#include <iostream>
#include "src/common/io/FileReader.h"
#include "src/utils/UtilFunctions.h"
#include "src/common/io/ObjectsReader.h"
#include <cassert>
#include <fstream>
#include "src/actors/Port.h"
#include "src/utils/Printers.h"
#include "tests/io_tests/IOTests.h"

using namespace std;


int main() {
    readCargoToPortFromFileTest();
}