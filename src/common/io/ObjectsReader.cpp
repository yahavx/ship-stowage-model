//
// Created by t-yabeny on 4/16/2020.
//

#include "ObjectsReader.h"
#include "FileReader.h"
#include "../../utils/UtilFunctions.h"
#include "../../utils/Printers.h"
#include <tuple>


std::optional<ShipPlan> readShipPlanFromFile(const std::string &filePath) {
    std::cout << "Attempting to read ship plan..." << std::endl;
    StringStringVector data = readFile(filePath);

    if (!isDataOnlyIntegers(data)) {  // couldn't convert data to int
        std::cout << "Error: data contain non-integers" << std::endl;
        return std::nullopt;
    }

    ShipPlan shipPlan;

    IntIntVector intData = convertDataToInt(data);
    IntVector firstRow = intData[0];
    if (firstRow.size() < 3) {
        std::cout << "Error: insufficient number of arguments for ship dimensions, exiting" << std::endl;
        return std::nullopt;
    }

    int z = firstRow[0], x = firstRow[1], y = firstRow[2];  // TODO: Changed from x, y ,z to z, x, y, validate it didn't ruin anything
    std::tuple<int, int, int> dimensions(x, y, z);
    shipPlan.setDimensions(dimensions);

    IntIntVector heights(x, IntVector(y, 0));  // init matrix of size (x,y) with zeroes

    for (longUInt i = 1; i < intData.size(); i++) {  // iterate on rows
        IntVector &intDataRow = intData[i];
        if (intDataRow.size() < 3) {
            std::cout << "Warning: data row contains less than 3 arguments, ignoring" << std::endl;
            continue;
        }

        int n = intDataRow[0];
        int m = intDataRow[1];
        int availableContainers = intDataRow[2];

        if (n < 0 || n >= x || m < 0 || m >= y) {
            std::cout << "Warning: data row exceeds the ship dimensions, ignoring" << std::endl;
            continue;
        }

        if (availableContainers >= z) {
            std::cout << "Warning: data row exceeds the maximum available containers, ignoring" << std::endl;
            continue;
        }

        heights[n][m] = z - availableContainers;  // we save the minimum available floor (z is the top)
    }

    shipPlan.setHeights(heights);

    std::cout << "Read ship plan successfully." << std::endl;

    return shipPlan;
}

std::optional<ShipRoute> readShipRouteFromFile(const std::string &filePath) {
    std::cout << "Attempting to read ship route..." << std::endl;
    StringStringVector data = readFile(filePath);

    std::vector<PortId> ports;
    std::string previousPort;  // to check that the same port doesn't appear twice

    for (StringVector dataRow : data) {
        std::string token = dataRow[0];  // ignore extra tokens in a row

        if (!isEnglishWord(token) || token.length() != 5) {
            std::cout << "Warning: invalid port format, ignoring" << std::endl;
            continue;
        }

        if (token.compare(previousPort) == 0) {
            std::cout << "Warning: same port appears twice in a row, ignoring" << std::endl;
            continue;
        }

        previousPort = token;

        ports.push_back(PortId(token));
    }

    std::cout << "Read ship route successfully." << std::endl;

    ShipRoute shipRoute(ports);
    return shipRoute;
}

std::optional<Port> readCargoToPortFromFile(const std::string &filePath) {
    std::cout << "Attempting to read cargo data..." << std::endl;

    std::string fileName = extractFilenameFromPath(filePath, false);  // false keeps the .cargo_data
    if (!isCargoDataFileFormat(fileName)) {
        std::cout << "Error: filename is in incorrect format, exiting" << std::endl;
        return std::nullopt;
    }

    PortId portId = PortId(fileName.substr(0, 5));
    Port port = Port(portId);
    Containers containersToAdd;

    StringStringVector data = readFile(filePath);

    for (StringVector dataRow : data) {
        if (dataRow.size() < 3) {
            std::cout << "Warning: data row contains less than 3 arguments, ignoring" << std::endl;
            continue;
        }

        std::string id = dataRow[0], weight = dataRow[1], destPort = dataRow[2];

        if (!Port::isIdInIsoFormat(id)) {
            std::cout << "Warning: container id not in ISO format, ignoring" << std::endl;
            continue;
        }

        if (!isInteger(weight)) {
            std::cout << "Warning: container weight is not an integer, ignoring" << std::endl;
            continue;
        }

        if (!isEnglishWord(destPort) || destPort.length() != 5) {
            std::cout << "Warning: port symbol is invalid, ignoring" << std::endl;
            continue;
        }

        containersToAdd.push_back(Container(id, stringToInt(weight), PortId(destPort)));
    }

    std::cout << "Read cargo data successfully." << std::endl;

    port.addContainers(containersToAdd);

    return port;
}

std::optional<OPS> readPackingOperationsFromFile(const std::string &filePath) {
    std::cout << "Attempting to read operations..." << std::endl;
    StringStringVector data = readFile(filePath);

    OPS operations;

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
        int floor = stringToInt(floorStr), x = stringToInt(xStr), y = stringToInt(yStr);

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
        int floor2 = stringToInt(floorStr2), x2 = stringToInt(xStr2), y2 = stringToInt(yStr2);

        operations.push_back(PackingOperation(packingType, containerId, {floor, x, y}, {floor2, x2, y2}));
    }

    if (operations.size() == 0) {
        std::cout << "Error: failed to read any operation" << std::endl;
        return std::nullopt;
    }

    std::cout << "Read operations successfully." << std::endl;
    return operations;
}

bool writePackingOperationsToFile(const std::string &filePath, OPS &operations) {
    StringStringVector data;

    std::cout << "Attempting to write operations..." << std::endl;

    for (PackingOperation op : operations) {
        data.emplace_back();  // add new row
        StringVector &currRow = data.back();

        currRow.push_back(packingTypeFromString(op.getType()));  // add operation type (L, U, M, R)
        currRow.push_back(op.getContainerId());  // add container id

        POS fromPos = op.getFromPosition();  // add fromPosition
        currRow.push_back(intToString(std::get<0>(fromPos)));
        currRow.push_back(intToString(std::get<1>(fromPos)));
        currRow.push_back(intToString(std::get<2>(fromPos)));

        if (op.getType() == PackingType::move) {
            POS toPos = op.getFromPosition();  // add toPosition (if move operation)
            currRow.push_back(intToString(std::get<0>(toPos)));
            currRow.push_back(intToString(std::get<1>(toPos)));
            currRow.push_back(intToString(std::get<2>(toPos)));
        }
    }

    bool res = writeFile(filePath, data);

    if (!res) {
        std::cout << "Error: couldn't write to file." << std::endl;
        return false;
    }
    std::cout << "Wrote operations successfully." << std::endl;
    return true;
}