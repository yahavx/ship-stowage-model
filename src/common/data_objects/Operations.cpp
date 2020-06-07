//
// Created by yahav on 14/05/2020.
//

#include <iostream>
#include "Operations.h"

// region Functions

void Operations::addOperation(const PackingOperation &op) {
    ops.push_back(op);
}

void Operations::addOperations(const Operations &otherOps) {
    ops.insert(ops.end(), otherOps.ops.begin(), otherOps.ops.end());
}

void Operations::addRejectOperations(const StringVector &containerIds) {
    for (auto& contId: containerIds) {
        ops.push_back({PackingType::reject, contId});
    }
}

int Operations::cost() const {
    int sum = 0;

    for (auto& op: ops) {
        switch (op.getType()) {
            case PackingType::reject:
                sum += 0;
                break;
            case PackingType::move:
                sum += 3;
                break;
            case PackingType::load:
                sum += 5;
                break;
            case PackingType::unload:
                sum += 5;
                break;
        }
    }

    return sum;
}

int Operations::size(bool excludeRejects) const {
    if (!excludeRejects)
        return ops.size();

    int count = 0;

    for (auto& op : ops) {
        if (op.getType() != PackingType::reject) {
            count++;
        }
    }

    return count;
}

bool Operations::empty() const {
    return ops.empty();
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Operations &ops) {

    if (ops.size() == 0) {
        os << "\t<NoOperations>" << std::endl;
        return os;
    }
//    std::cout << "Operations {" << std::endl;
    for (int i = 0; i < ops.size(); i++) {
        os << '\t' << ops.ops[i].toString() << std::endl;
    }
//    std::cout << "}" << std::endl;
    return os;
}

// endregion
