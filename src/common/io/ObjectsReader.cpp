//
// Created by t-yabeny on 4/16/2020.
//

#include "ObjectsReader.h"
#include "FileReader.h"
#include "../utils/UtilFunctions.h"
#include "../utils/Printers.h"
#include "../utils/Errors.h"
#include <tuple>
#include <unordered_map>
#include <set>


// This is a bit ugly, but will carry on for now
const std::string shipPlanWarning = "\t[Ship Plan Error] ";
const std::string shipRouteWarning = "\t[Ship Route Error] ";
const std::string cargoDataWarning = "\t[Cargo Data Error] ";
const std::string containersAtPortWarning = "\t[Containers At Port Error] ";
const std::string operationsWarning = "\t[Algorithm Warning] ";  // this relates to the file that the algorithm writes


/**
 * Adds a warning if a data row has too many parameters.
 *
 * @param dataRow a data row, represented by a vector of tokens.
 * @param errorPrefix since this is used by each of the functions below, the error prefix can't be hard coded, so this will indicate it.
 * @param expected number of parameters expected.
 * @param lineNum line number to document if there's an error.
 * @param errors the error will be appended to this, if needed.
 */
void addExtraParametersWarningIfNeeded(const StringVector &dataRow, const std::string &errorPrefix, int expected, int lineNum, Errors &errors) {
    int actual = dataRow.size();
    if (actual > expected) {
        errors.addError({FileInput_TooManyParameters, errorPrefix, intToStr(lineNum + 1), intToStr(expected), intToStr(actual)});
    }
}

ShipPlan readShipPlanFromFile(const std::string &filePath, Errors &errors) {
    StringStringVector data = readFile(filePath);
    ShipPlan shipPlan;
    std::map<std::pair<int, int>, int> positionsWithData;  // To track duplicates

    longUInt i;
    for (i = 0; i < data.size() && data[i].empty(); i++);  // Find first non empty line

    if (i >= data.size()) {  // No data rows (only white space lines, or starting with #)
        errors.addError(ErrorFlag::ShipPlan_FatalError_NoFileOrInvalidFirstLine);
        return shipPlan;
    }

    StringVector &firstRow = data[i];  // First row with actual data
    if (firstRow.size() < 3 || !isRowOnlyIntegers(firstRow, 3)) {
        errors.addError({ErrorFlag::ShipPlan_FatalError_NoFileOrInvalidFirstLine, intToStr(i)});
        return shipPlan;
    }

    addExtraParametersWarningIfNeeded(firstRow, shipPlanWarning, 3, i, errors);
    IntVector firstIntRow = convertRowToInt(firstRow, 3);

    int z = firstIntRow[0], x = firstIntRow[1], y = firstIntRow[2];
    shipPlan.setDimensions({x, y, z});

    IntIntVector heights(x, IntVector(y, 0));  // init matrix of size (x,y) with zeroes

    for (i = i + 1; i < data.size(); i++) {  // iterate on rows
        StringVector &dataRow = data[i];

        if (dataRow.size() == 0)
            continue;

        if (dataRow.size() < 3) {
            errors.addError({ErrorFlag::ShipPlan_BadLineFormat, intToStr(i+1)});
            continue;
        }

        addExtraParametersWarningIfNeeded(dataRow, shipPlanWarning, 3, i, errors);

        if (!isRowOnlyIntegers(dataRow, 3)) {  // Check the first 3
            errors.addError({ErrorFlag::ShipPlan_BadLineFormat, intToStr(i+1)});
            continue;
        }

        IntVector intDataRow = convertRowToInt(dataRow, 3);

        int n = intDataRow[0];
        int m = intDataRow[1];
        int availableContainers = intDataRow[2];

        if (n < 0 || n >= x || m < 0 || m >= y) {
            errors.addError({ErrorFlag::ShipPlan_InvalidXYCoordinates, intToStr(i+1), intToStr(n), intToStr(m)});
            continue;
        }

        auto it = positionsWithData.find({n, m});

        if (it != positionsWithData.end()) {
            if (it->second == availableContainers) {  // Duplicate data about (x,y), but with same z
                errors.addError({ErrorFlag::ShipPlan_BadLineFormat, intToStr(i+1), intToStr(n), intToStr(m)});
                continue;
            }
            // else
            errors.addError({ErrorFlag::ShipPlan_FatalError_DuplicateData, intToStr(i+1), intToStr(n), intToStr(m)});
            return shipPlan;
        }

        positionsWithData.insert({{n, m}, availableContainers});  // Add position to hash table, to later check for duplicates

        if (availableContainers >= z) {
            errors.addError({ErrorFlag::ShipPlan_InvalidFloorHeight, intToStr(i+1), intToStr(availableContainers), intToStr(z)});
            continue;
        }

        heights[n][m] = z - availableContainers;  // We save the minimum available floor (z is the top)
    }

    shipPlan.setHeights(heights);
    return shipPlan;
}

ShipRoute readShipRouteFromFile(const std::string &filePath, Errors &errors) {
    StringStringVector data = readFile(filePath);
    ShipRoute shipRoute;

    if (data.empty()) {
        errors.addError(ErrorFlag::ShipRoute_FatalError_NoFileOrNoLegalPorts);  // no data read, or couldn't open file
        return shipRoute;
    }

    std::vector<PortId> ports;

    std::string previousPort;

    for (longUInt i = 0; i < data.size(); i++) {
        StringVector &dataRow = data[i];

        if (dataRow.empty()){
            continue;
        }

        std::string token = toUpper(dataRow[0]);

        addExtraParametersWarningIfNeeded(dataRow, shipRouteWarning, 1, i, errors);

        if (!isEnglishWord(token) || token.length() != 5) {
            errors.addError({ErrorFlag::ShipRoute_BadPortSymbol, intToStr(i+1), token});
            continue;
        }

        if (token == previousPort) {
            errors.addError({ErrorFlag::ShipRoute_TwoConsecutiveSamePort, intToStr(i+1), token});
//            std::cout << "Warning: same port appears twice in a row, ignoring" << std::endl;
            continue;
        }

        previousPort = token;

        ports.push_back(PortId(token));
    }

    if (ports.size() == 1) {
        errors.addError({ErrorFlag::ShipRoute_FatalError_SinglePort, ports[0]});
        return shipRoute;
    }

    if (ports.size() == 0) {
        errors.addError(ErrorFlag::ShipRoute_FatalError_NoFileOrNoLegalPorts);
        return shipRoute;
    }

    shipRoute.setPorts(ports);
    return shipRoute;
}

ContainerStorage readPortCargoFromFile(const std::string &filePath, Errors &errors) {
    Containers containers;

    if (!isFileExist(filePath)) {
        errors.addError({ErrorFlag::CargoData_InvalidFile, extractFilenameFromPath(filePath)});
        return containers;
    }

    StringStringVector data = readFile(filePath);

    for (longUInt i = 0; i < data.size(); i++) {
        auto &dataRow = data[i];

        if (dataRow.empty())
            continue;

        std::string id, weight, destPort;

        if (dataRow.size() > 0) {
            id = dataRow[0];
        }
        if (dataRow.size() > 1) {
            weight = dataRow[1];
        }
        if (dataRow.size() > 2) {
            destPort = dataRow[2];
        }

        if (data.size() > 2) {
            addExtraParametersWarningIfNeeded(dataRow, cargoDataWarning, 3, i, errors);
        }

        if (id == "") {
            errors.addError({ErrorFlag::CargoData_MissingContainerID, intToStr(i+1)});
            continue;
        }

        int weightInt = isInteger(weight) ? strToInt(weight) : -1;
        containers.push_back(Container(id, weightInt, PortId(destPort)));
    }

    return ContainerStorage(containers);
}

Operations readPackingOperationsFromFile(const std::string &filePath, Errors &errors) {
    Operations operations;

    if (!isFileExist(filePath)) {
        errors.addError(ErrorFlag::ReadOperations_InvalidFile);
        return operations;
    }

    StringStringVector data = readFile(filePath);

    for (longUInt i = 0; i < data.size(); i++) {
        auto &dataRow = data[i];

        if (dataRow.empty())
            continue;

        if (dataRow.size() < 2) {
            errors.addError({ErrorFlag::ReadOperations_InsufficientRowData, intToStr(i)});
            continue;
        }

        std::string opStr = dataRow[0];

        if (opStr.size() != 1 || (opStr[0] != 'L' && opStr[0] != 'U' && opStr[0] != 'M' && opStr[0] != 'R')) {
            errors.addError({ErrorFlag::ReadOperations_InvalidOperationType, intToStr(i), opStr});
            continue;
        }
        PackingType packingType = packingTypeToString(opStr[0]);
        std::string containerId = dataRow[1];  // Str suffix indicates its string representation

        if (packingType == PackingType::reject) {
            operations.addOperation({packingType, containerId});
//            addExtraParametersWarningIfNeeded(dataRow, operationsWarning, 5, i, errors);  // not sure what to except
            continue;
        }

        if (dataRow.size() < 5) {
            errors.addError({ErrorFlag::ReadOperations_InsufficientRowData, intToStr(i)});
            continue;
        }

        if (packingType == PackingType::load || packingType == PackingType::unload) {
            addExtraParametersWarningIfNeeded(dataRow, operationsWarning, 5, i, errors);
        }

        std::string floorStr = dataRow[2], xStr = dataRow[3], yStr = dataRow[4];

        if (!isInteger(floorStr) || !isInteger(xStr) || !isInteger(yStr)) {
            if (!isInteger(floorStr)) {
                errors.addError({ErrorFlag::ReadOperations_InvalidShipPosition,  intToStr(i), "floor", floorStr});
            }
            if (!isInteger(xStr)) {
                errors.addError({ErrorFlag::ReadOperations_InvalidShipPosition, intToStr(i), "x", xStr});
            }
            if (!isInteger(yStr)) {
                errors.addError({ErrorFlag::ReadOperations_InvalidShipPosition, intToStr(i), "y", yStr});
            }
            continue;
        }

        int floor = strToInt(floorStr), x = strToInt(xStr), y = strToInt(yStr);

        if (packingType == PackingType::load || packingType == PackingType::unload) {  // Load or Unload, we have all the arguments needed
            operations.addOperation({packingType, containerId, {x, y, floor}});
            continue;
        }

        if (dataRow.size() < 8) {
            errors.addError({ErrorFlag::ReadOperations_InsufficientRowData, intToStr(i)});
            continue;
        }

        addExtraParametersWarningIfNeeded(dataRow, operationsWarning, 8, i, errors);
        std::string floorStr2 = dataRow[5], xStr2 = dataRow[6], yStr2 = dataRow[7];

        if (!isInteger(floorStr2) || !isInteger(xStr2) || !isInteger(yStr2)) {
            if (!isInteger(floorStr2)) {
                errors.addError({ErrorFlag::ReadOperations_InvalidShipPosition, intToStr(i),"floor", floorStr2});
            }
            if (!isInteger(xStr2)) {
                errors.addError({ErrorFlag::ReadOperations_InvalidShipPosition, intToStr(i), "x", xStr2});
            }
            if (!isInteger(yStr2)) {
                errors.addError({ErrorFlag::ReadOperations_InvalidShipPosition, intToStr(i), "y", yStr2});
            }
            continue;
        }

        int floor2 = strToInt(floorStr2), x2 = strToInt(xStr2), y2 = strToInt(yStr2);

        operations.addOperation({packingType, containerId, {x, y, floor}, {x2, y2, floor2}});
    }

    return operations;
}

bool writePackingOperationsToFile(const std::string &filePath, Operations &operations) {
    StringStringVector data;

    for (PackingOperation &op : operations.ops) {
        data.emplace_back();  // add new row
        StringVector &currRow = data.back();

        currRow.push_back(packingTypeFromString(op.getType()));  // add operation type (L, U, M, R)
        currRow.push_back(op.getContainerId());  // add container id

        if (op.getType() == PackingType::reject) {  // we need 2 parameters
            continue;
        }

        const Position &fromPos = op.getFirstPosition();  // add pos1
        currRow.push_back(intToStr(fromPos.floor()));
        currRow.push_back(intToStr(fromPos.X()));
        currRow.push_back(intToStr(fromPos.Y()));

        if (op.getType() == PackingType::move) {
            const Position &toPos = op.getSecondPosition();  // add pos2
            currRow.push_back(intToStr(toPos.floor()));
            currRow.push_back(intToStr(toPos.X()));
            currRow.push_back(intToStr(toPos.Y()));
        }
    }

    bool res = writeFile(filePath, data);

    if (!res) {
        return false;
    }

    return true;
}