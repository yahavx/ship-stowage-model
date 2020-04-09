#include <iostream>
#include "src/data_objects/Container.h"
#include <fstream>
#include "src/io/InputOutput.h"
#include "src/actors/Port.h"

using namespace std;

int main() {
    Container cont(2, PortId("TAU"), "d");
    Port port(PortId("TAU"));
    cout << cont << endl;
    cout << port << endl;
}
