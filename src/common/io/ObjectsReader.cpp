//
// Created by t-yabeny on 4/16/2020.
//

#include "ObjectsReader.h"
#include "FileReader.h"
#include "../utils/UtilFunctions.h"
#include "../utils/Printers.h"
#include "../utils/Errors.h"
#include <tuple>

//#define DEBUG

// general for this module:
// TODO: remove all prints when we are sure we don't need them
// TODO: check if we need to have in each data row exactly X tokens, or we can have extra and ignore them
// TODO: match the correctness checks accordingly
// TODO: for each error, we just say yes or no, maybe we want to count them instead

ShipPlan readShipPlanFromFile(const std::string &filePath, Errors &errors) {
#ifdef DEBUG
    std::cout << "Attempting to read ship plan..." << std::endl;
#endif
    StringStringVector data = readFile(filePath);
    ShipPlan shipPlan;

    if (data.size() == 0) {
        errors.addError(ErrorFlag::ShipPlan_FatalError);  // no data read, or couldn't open file
        return shipPlan;
    }

    StringVector &firstRow = data[0];
    if (firstRow.size() < 3 || !isRowOnlyIntegers(firstRow)) {
        errors.addError(ErrorFlag::ShipPlan_FatalError);
//        std::cerr << "Error: insufficient number of arguments for ship dimensions, exiting" << std::endl;
        return shipPlan;
    }

    IntVector firstIntRow = convertRowToInt(firstRow);

    int z = firstIntRow[0], x = firstIntRow[1], y = firstIntRow[2];
    shipPlan.setDimensions({x, y, z});

    IntIntVector heights(x, IntVector(y, 0));  // init matrix of size (x,y) with zeroes

    for (longUInt i = 1; i < data.size(); i++) {  // iterate on rows
        StringVector &dataRow = data[i];

        if (dataRow.size() < 3) {
            errors.addError(ErrorFlag::ShipPlan_BadLineFormat);
//            std::cout << "Warning: data row contains less than 3 arguments, ignoring" << std::endl;
            continue;
        }

        if (!isRowOnlyIntegers(dataRow)) {
            errors.addError(ErrorFlag::ShipPlan_BadLineFormat);
            continue;
        }

        IntVector intDataRow = convertRowToInt(dataRow);

        int n = intDataRow[0];
        int m = intDataRow[1];
        int availableContainers = intDataRow[2];

        if (n < 0 || n >= x || m < 0 || m >= y) {
            errors.addError(ErrorFlag::ShipPlan_InvalidXYCoordinates);
//            std::cout << "Warning: data row exceeds the ship dimensions, ignoring" << std::endl;
            continue;
        }

        if (availableContainers >= z) {
            errors.addError(ErrorFlag::ShipPlan_InvalidFloorHeight);
//            std::cout << "Warning: data row exceeds the maximum available containers, ignoring" << std::endl;
            continue;
        }

        heights[n][m] = z - availableContainers;  // we save the minimum available floor (z is the top)
    }

    shipPlan.setHeights(heights);
#ifdef DEBUG
    std::cout << "Read ship plan successfully." << std::endl;
#endif
    return shipPlan;
}

ShipRoute readShipRouteFromFile(const std::string &filePath, Errors &errors) {
#ifdef DEBUG
    std::cout << "Attempting to read ship route..." << std::endl;
#endif
    StringStringVector data = readFile(filePath);
    ShipRoute shipRoute;

    if (data.size() == 0) {
        errors.addError(ErrorFlag::ShipRoute_FatalError);  // no data read, or couldn't open file
        return shipRoute;
    }

    std::vector<PortId> ports;

    std::string previousPort;

    for (StringVector dataRow : data) {
        std::string token = dataRow[0];  // ignore extra tokens in a row

        if (!isEnglishWord(token) || token.length() != 5) {
            errors.addError(ErrorFlag::ShipRoute_BadPortSymbol);
//            std::cout << "Warning: invalid port format (" << token << "), ignoring" << std::endl;
            continue;
        }

        if (token == previousPort) {
            errors.addError(ErrorFlag::ShipRoute_TwoConsecutiveSamePort);
//            std::cout << "Warning: same port appears twice in a row, ignoring" << std::endl;
            continue;
        }

        previousPort = token;

        ports.push_back(PortId(token));
    }

    if (ports.size() == 1) {
        errors.addError(ErrorFlag::ShipRoute_FatalError_SinglePort);
//        std::cerr << "Error: read only one port" << std::endl;
        return shipRoute;
    }

    if (ports.size() == 0) {
        errors.addError(ErrorFlag::ShipRoute_FatalError);
//        std::cerr << "Error: couldn't read any port from route file" << std::endl;
        return shipRoute;
    }

#ifdef DEBUG
    std::cout << "Read ship route successfully." << std::endl;
#endif
    shipRoute.setPorts(ports);
    return shipRoute;
}

ContainerStorage readPortCargoFromFile(const std::string &filePath, Errors &errors) {
#ifdef DEBUG
    std::cout << "Attempting to read cargo data..." << std::endl;
#endif
//    std::string fileName = extractFilenameFromPath(filePath, false);  // false keeps the .cargo_data  // TODO: we assume this can't happen now (we will send only valid files), remove this when we are sure
//    if (!isCargoDataFileFormat(fileName)) {
//        std::cerr << "Error: filename is in incorrect format, exiting" << std::endl;
//        return std::nullopt;
//    }

    StringStringVector data = readFile(filePath);
    Containers containers;

    if (data.size() == 0) {
        errors.addError(ErrorFlag::CargoData_InvalidFile);
        return containers;
    }

    for (StringVector dataRow : data) {
        if (dataRow.size() < 3) {
            errors.addError(ErrorFlag::CargoData_MissingOrBadPortDest);  // the port dest is last - so he must be missing
//            std::cout << "Warning: data row contains less than 3 arguments, ignoring" << std::endl;
            continue;
        }

        std::string id = dataRow[0], weight = dataRow[1], destPort = dataRow[2];

        if (!Port::isIdInIsoFormat(id)) {
            errors.addError(ErrorFlag::CargoData_BadContainerID);
//            std::cout << "Warning: container id not in ISO format, ignoring" << std::endl;
            continue;
        }

        if (!isInteger(weight)) {
            errors.addError(ErrorFlag::CargoData_MissingOrBadWeight);
//            std::cout << "Warning: container weight is not an integer, ignoring" << std::endl;
            continue;
        }

        if (!isEnglishWord(destPort) || destPort.length() != 5) {
            errors.addError(ErrorFlag::CargoData_MissingOrBadPortDest);
//            std::cout << "Warning: port symbol is invalid, ignoring" << std::endl;
            continue;
        }

        containers.push_back(Container(id, strToInt(weight), PortId(destPort)));
    }

#ifdef DEBUG
    std::cout << "Read cargo data successfully." << std::endl;
#endif

    return ContainerStorage(containers);
}

std::optional<Operations> readPackingOperationsFromFile(const std::string &filePath) {
#ifdef DEBUG
    std::cout << "Attempting to read operations..." << std::endl;
#endif
    StringStringVector data = readFile(filePath);

    Operations operations;

    for (StringVector dataRow : data) {
        if (dataRow.size() < 5) {
            std::cout << "Warning: data row contains less than 5 arguments, ignoring" << std::endl;
            continue;
        }

        std::string opStr = dataRow[0], containerId = dataRow[1];  // Str suffix indicates its string representation
        std::string floorStr = dataRow[2], xStr = dataRow[3], yStr = dataRow[4];

        if (opStr.size() != 1 || (opStr[0] != 'L' && opStr[0] != 'U' && opStr[0] != 'M' && opStr[0] != 'R')) {
            std::cout << "Warning: invalid operation type, ignoring" << std::endl;
            continue;
        }
        PackingType packingType = packingTypeToString(opStr[0]);

        if (packingType == PackingType::move && dataRow.size() < 8) {
            std::cout << "Warning: data row contains less than 8 arguments for move operation, ignoring" << std::endl;
            continue;
        }

        if (!Port::isIdInIsoFormat(containerId)) {
            std::cout << "Warning: container id not in ISO format, ignoring" << std::endl;
            continue;
        }

        if (!isInteger(floorStr) || !isInteger(xStr) || !isInteger(yStr)) {
            std::cout << "Warning: floor, x or y are not an integers, ignoring" << std::endl;
            continue;
        }
        int floor = strToInt(floorStr), x = strToInt(xStr), y = strToInt(yStr);

        if (packingType != PackingType::move) {  // We have all the arguments needed
            operations.push_back(PackingOperation(packingType, containerId, {floor, x, y}));
            continue;
        }

        // It's a move operation

        std::string floorStr2 = dataRow[5], xStr2 = dataRow[6], yStr2 = dataRow[7];

        if (!isInteger(floorStr2) || !isInteger(xStr2) || !isInteger(yStr2)) {
            std::cout << "Warning: floor, x or y are not an integers, ignoring" << std::endl;
            continue;
        }
        int floor2 = strToInt(floorStr2), x2 = strToInt(xStr2), y2 = strToInt(yStr2);

        operations.push_back(PackingOperation(packingType, containerId, {floor, x, y}, {floor2, x2, y2}));
    }

    if (operations.empty()) {
        std::cerr << "Warning: failed to read any operation" << std::endl;
        return std::nullopt;
    }

#ifdef DEBUG
    std::cout << "Read operations successfully." << std::endl;
#endif
    return operations;
}

bool writePackingOperationsToFile(const std::string &filePath, Operations &operations) {
    StringStringVector data;
#ifdef DEBUG
    std::cout << "Attempting to write operations..." << std::endl;
#endif
    for (PackingOperation op : operations) {
        data.emplace_back();  // add new row
        StringVector &currRow = data.back();

        currRow.push_back(packingTypeFromString(op.getType()));  // add operation type (L, U, M, R)
        currRow.push_back(op.getContainerId());  // add container id

        POS fromPos = op.getFromPosition();  // add fromPosition
        currRow.push_back(intToStr(std::get<0>(fromPos)));
        currRow.push_back(intToStr(std::get<1>(fromPos)));
        currRow.push_back(intToStr(std::get<2>(fromPos)));

        if (op.getType() == PackingType::move) {
            POS toPos = op.getFromPosition();  // add toPosition (if move operation)
            currRow.push_back(intToStr(std::get<0>(toPos)));
            currRow.push_back(intToStr(std::get<1>(toPos)));
            currRow.push_back(intToStr(std::get<2>(toPos)));
        }
    }

    bool res = writeFile(filePath, data);

    if (!res) {
        std::cerr << "Error: couldn't write to file." << std::endl;
        return false;
    }
#ifdef DEBUG
    std::cout << "Wrote operations successfully." << std::endl;
#endif
    return true;
}