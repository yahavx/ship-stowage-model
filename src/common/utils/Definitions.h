//
// Created by t-yabeny on 4/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_DEFINITIONS_H
#define SHIP_STOWAGE_MODEL_DEFINITIONS_H

#include <string>
#include <vector>
#include <map>
#include "../data_objects/Container.h"
#include "../data_objects/PackingOperation.h"


/// Popular typedefs/structs

class PackingOperation;

typedef std::vector<int> IntVector;

typedef std::vector<std::vector<int>> IntIntVector;

typedef std::vector<std::string> StringVector;

typedef std::vector<std::vector<std::string>> StringStringVector;

typedef std::tuple<int, int, int> POS;

typedef std::vector<PackingOperation> OPS;

typedef std::map<std::string, std::vector<std::string>> StringToStringVectorMap;

typedef std::map<std::string, int> StringToIntMap;

typedef std::vector<Container> Containers;

typedef std::optional<Container> OptionalContainer;

typedef long long unsigned int longUInt;


#endif //SHIP_STOWAGE_MODEL_DEFINITIONS_H
