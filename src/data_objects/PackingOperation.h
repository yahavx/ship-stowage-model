//
// Created by yahav on 05/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
#define SHIP_STOWAGE_MODEL_PACKINGOPERATION_H

#include <string>
#include <tuple>
#include <ostream>

enum class PackingType {
    load, unload, reject
};

/// Instructions for loading and unloading cargo to the ship
class PackingOperation {
    std::string containerId;

    std::tuple<int, int, int> fromPosition;

    std::tuple<int, int, int> toPosition;

    PackingType type;

public:
    // Constructors
    PackingOperation(const std::string &containerId, const std::tuple<int, int, int> &fromPosition,
                     const std::tuple<int, int, int> &toPosition, PackingType type);


    //Getters and setters
    const std::string &getContainerId() const;

    const std::tuple<int, int, int> &getFromPosition() const;

    const std::tuple<int, int, int> &getToPosition() const;

    PackingType getType() const;


    // Printer
    friend std::ostream &operator<<(std::ostream &os, const PackingOperation &operation);
};


#endif //SHIP_STOWAGE_MODEL_PACKINGOPERATION_H
