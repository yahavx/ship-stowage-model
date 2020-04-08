#include <iostream>
#include "src/Container.h"
#include <fstream>
#include "src/InputOutput.h"

using namespace std;

int main() {
    Container cont(2, Port(), "d");
    cout << cont << endl;

    // basic input output

    write("blabla\nblafsdfbla");

    read();
}
