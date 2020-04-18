//
// Created by Orr on 4/16/2020.
//

#include "Cargo.h"

bool validateXY(int x, int y, const ShipPlan &shipPlan) {
    POS dims = shipPlan.getDimensions();
    if (x < 0 || x >= std::get<0>(dims))
        return false;

    if (y < 0 || y >= std::get<1>(dims))
        return false;

    return true;
}

Cargo::Cargo(const ShipPlan &shipPlan) : shipPlan(shipPlan) {
    POS dims = shipPlan.getDimensions();
    auto x = std::get<0>(dims), y = std::get<1>(dims);
    this->containers = std::vector<std::vector<Containers>>(x, std::vector<Containers>(y, Containers()));
}

OptionalContainer Cargo::getTopContainer(int x, int y) const {
    if (!validateXY(x, y, shipPlan))
        return {};
    Containers xyContainers = containers[x][y];
    if (xyContainers.empty())
        return {};

    return xyContainers.back();
}

OptionalContainer Cargo::removeTopContainer(int x, int y) {
    if (!validateXY(x, y, shipPlan))
        return {};
    Containers xyContainers = containers[x][y];
    if (xyContainers.empty())
        return {};

    const Container &container = xyContainers.back();
    xyContainers.pop_back();
    return container;
}

int Cargo::canLoadContainerOnTop(int x, int y, const Container &container) const {
    if (!validateXY(x, y, shipPlan))
        return -1;

    int maxHeight = shipPlan.getHeights()[x][y];
    int currentHeight = currentNumContainers(x, y);
    if (currentHeight >= maxHeight)
        return -1;

    Containers xyContainers = containers[x][y];
    int loadedToHeight = containers.size();

    return loadedToHeight;
    std::cout << container;  // TODO: remove (need this parameter?)
    return loadedToHeight;
}

int Cargo::loadContainerOnTop(int x, int y, const Container &container) {
    if (!validateXY(x, y, shipPlan))
        return -1;

    int maxHeight = shipPlan.getHeights()[x][y];
    int currentHeight = currentNumContainers(x, y);
    if (currentHeight >= maxHeight)
        return -1;

    Containers xyContainers = containers[x][y];
    int loadedToHeight = containers.size();
    xyContainers.push_back(container);

    return loadedToHeight;
}

std::vector<ContainerPosition> Cargo::getContainersForPort(const PortId &portId) const {
    std::vector<ContainerPosition> result = std::vector<ContainerPosition>();

    POS dims = shipPlan.getDimensions();
    for (int x = 0; x < std::get<0>(dims); x++)
        for (int y = 0; y < std::get<1>(dims); y++) {
            Containers xyContainers = containers[x][y];

            for (longUInt z = 0; z < xyContainers.size(); z++)
                if (xyContainers[z].getDestPort() == portId) {
                    result.push_back(ContainerPosition(xyContainers[z], {x, y, z}));
                }
        }


    return result;
}

int Cargo::currentNumContainers(int x, int y) const {
    if (!validateXY(x, y, shipPlan))
        return -1;

    Containers xyContainers = containers[x][y];
    return xyContainers.size();
}

