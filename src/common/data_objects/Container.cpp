//
// Created by yahav on 05/04/2020.
//

#include <regex>
#include <cmath>
#include "Container.h"
#include "../utils/UtilFunctions.h"

#include <utility>


// region Constructors

Container::Container(const std::string &id, int weight, const PortId &destPort) : id(id), weight(weight),
                                                                                  destPort(destPort) {}
// endregion

// region Getters and setters

int Container::getWeight() const {
    return weight;
}

void Container::setWeight(int weight) {
    Container::weight = weight;
}

const PortId &Container::getDestPort() const {
    return destPort;
}

void Container::setDestPort(const PortId &destPort) {
    Container::destPort = destPort;
}

const std::string &Container::getId() const {
    return id;
}

void Container::setId(const std::string &id) {
    Container::id = id;
}

// endregion

// region Operators

bool Container::operator==(const Container &rhs) const {
    return id == rhs.id;
}

bool Container::operator!=(const Container &rhs) const {
    return !(rhs == *this);
}

// endregion

// region Functions

bool Container::isIdInIsoFormat() const {
    #ifdef SOFT_ISO_CHECK
    return id.length() == 11;
    #endif

    std::map<char, int> mymap = {{'A',10}, {'B',12}, {'C',13}, {'D',14}, {'E',15}, {'F',16}, {'G',17},
                                 {'H',18}, {'I',19}, {'J',20}, {'K',21}, {'L',23}, {'M',24}, {'N',25},
                                 {'O',26}, {'P',27}, {'Q',28}, {'R',29}, {'S',30}, {'T',31}, {'U',32},
                                 {'V',34}, {'W',35}, {'X',36}, {'Y',37}, {'Z',38}, {'0',0},{'1',1},
                                 {'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9}};

    std::regex reg = std::basic_regex("[A-Z]{3}[UZJ][0-9]{6}[0-9]");
    if (!std::regex_match(id, reg)) {
        return false;
    }
    double sum = 0;
    int n = (int)id.length() - 1;
    double tmp = 0.0;
    for (int i = 0; i < n; i++)
    {
        int u = mymap[id[i]] * pow(2,i);
        sum += u;
    }
    tmp = sum/11;
    tmp = floor(tmp);
    tmp = tmp * 11;

    int actual = sum-tmp;
    int expected = mymap[(id[n])];

    if (actual == 10) {
        actual = 0;
    }

    return  actual == expected;
}

Error Container::isContainerLegal() const {
    if (!isIdInIsoFormat()) {
        return {ErrorFlag::ContainersAtPort_BadContainerID, id};
    }

    if (weight <= 0) {
        return {ErrorFlag::ContainersAtPort_MissingOrBadWeight, id};
    }

    if (!destPort.isValid()) {
        return {ErrorFlag::ContainersAtPort_MissingOrBadPortDest, id};
    }

    return ErrorFlag::Success;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Container &container) {
    os << "Container(weight: " << container.weight << ", destPort: " << container.destPort << ", id: " << container.id
       << ")";
    return os;
}

// endregion