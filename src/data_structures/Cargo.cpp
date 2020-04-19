//
// Created by Orr on 4/16/2020.
//

#include "Cargo.h"


// region Constructors

Cargo::Cargo() {}

Cargo::Cargo(const ShipPlan &shipPlan) : shipPlan(shipPlan) {
    POS dims = shipPlan.getDimensions();
    auto x = std::get<0>(dims), y = std::get<1>(dims);
    this->containers = std::vector<std::vector<Containers>>(x, std::vector<Containers>(y, Containers()));
}
// endregion

// region Functions

bool validateXY(int x, int y, const ShipPlan &shipPlan) {
    POS dims = shipPlan.getDimensions();
    if (x < 0 || x >= std::get<0>(dims))
        return false;

    if (y < 0 || y >= std::get<1>(dims))
        return false;

    return true;
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

int Cargo::canLoadContainerOnTop(int x, int y) const {
    if (!validateXY(x, y, shipPlan))
        return -1;

    int maxHeight = std::get<2>(shipPlan.getDimensions());
    int currentHeight = currentTopHeight(x, y);
    if (currentHeight >= maxHeight)
        return -1;

    return currentHeight;
}

int Cargo::loadContainerOnTop(int x, int y, const Container &container) {
    if (!validateXY(x, y, shipPlan))
        return -1;

    int maxHeight = std::get<2>(shipPlan.getDimensions());
    int currentHeight = currentTopHeight(x, y);
    if (currentHeight >= maxHeight)
        return -1;

    Containers &xyContainers = containers[x][y];
    xyContainers.push_back(container);

    return currentHeight;
}

std::vector<ContainerPosition> Cargo::getContainersForPort(const PortId &portId) const {
    std::vector<ContainerPosition> result = std::vector<ContainerPosition>();

    POS dims = shipPlan.getDimensions();
    for (int x = 0; x < std::get<0>(dims); x++)
        for (int y = 0; y < std::get<1>(dims); y++) {
            Containers xyContainers = containers[x][y];

            for (longUInt z = 0; z < xyContainers.size(); z++)
                if (xyContainers[z].getDestPort() == portId) {
                    auto height = this->shipPlan.getHeights()[x][y] + z;
                    Container cont(xyContainers[z]);
                    result.push_back(ContainerPosition(cont, {x, y, height}));
                }
        }


    return result;
}

int Cargo::currentTopHeight(int x, int y) const {
    if (!validateXY(x, y, shipPlan))
        return -1;

    const Containers &xyContainers = containers[x][y];
    auto height = this->shipPlan.getHeights()[x][y] + xyContainers.size();
    return height;
}
// endregion

std::ostream &operator<<(std::ostream &os, const Cargo &cargo) {
    os << "CargoPrintNotImplemented: " << std::endl;
    return os;
    os << *cargo.getTopContainer(1, 1);
}