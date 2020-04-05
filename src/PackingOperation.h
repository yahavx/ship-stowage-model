//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
#define SHIP_STOWAGE_MODEL_PACKINGOPERATION_H

#include <string>

using namespace std;

enum class Packing{
    load, unload, reject
};

class PackingOperation {
    string containerId;
    int floorIndex;
    int rowIndex;
    int columnIndex;
};


#endif //SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
