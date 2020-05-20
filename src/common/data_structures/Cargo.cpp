//
// Created by Orr on 4/16/2020.
//

#include "Cargo.h"


// region Constructors

Cargo::Cargo() {}

Cargo::Cargo(const ShipPlan &shipPlan) : shipPlan(shipPlan) {
    Dimensions dims = shipPlan.getDimensions();
    auto x = dims.X(), y = dims.Y();
    this->containers = std::vector<std::vector<Containers>>(x, std::vector<Containers>(y, Containers()));
}

// endregion

// region boolean

bool Cargo::validateXY(int x, int y) const {
    Dimensions dims = shipPlan.getDimensions();
    if (x < 0 || x >= dims.X())
        return false;

    if (y < 0 || y >= dims.Y())
        return false;

    return true;
}

bool Cargo::canLoadContainerToPosition(int x, int y) const {
    return getAvailableFloorToLoadContainer(x, y) != -1;
}

bool Cargo::isFull() {
    const Dimensions &dims = shipPlan.getDimensions();
    for (int x = 0; x < dims.X(); x++)
        for (int y = 0; y < dims.Y(); y++) {
            Containers xyContainers = containers[x][y];
            if ((int) xyContainers.size() < dims.Z() - shipPlan.getHeights()[x][y])
                return false;
        }

    return true;
}

bool Cargo::isEmpty() {
    return containersMapping.empty();
}

bool Cargo::hasContainer(std::string containerId) const {
    return containersMapping.find(containerId) != containersMapping.end();
}

// endregion

// region Load/Unload

OptionalContainer Cargo::removeTopContainer(int x, int y) {
    if (!validateXY(x, y))
        return {};
    Containers &xyContainers = containers[x][y];
    if (xyContainers.empty())
        return {};

    const Container &container = xyContainers.back();
    xyContainers.pop_back();
    containersMapping.erase(container.getId());
    return container;
}

int Cargo::loadContainerOnTop(int x, int y, const Container &container) {
    if (!validateXY(x, y))
        return -1;

    int maxHeight = shipPlan.getDimensions().Z();
    int currentHeight = currentTopHeight(x, y);
    if (currentHeight >= maxHeight)
        return -1;

    Containers &xyContainers = containers[x][y];
    xyContainers.push_back(container);

    containersMapping.insert({container.getId(), container});
    return currentHeight;
}

// endregion

// region Others

int Cargo::getAvailableFloorToLoadContainer(int x, int y) const {
    if (!validateXY(x, y))
        return -1;

    int maxHeight = shipPlan.getDimensions().Z();
    int currentHeight = currentTopHeight(x, y);
    if (currentHeight >= maxHeight)
        return -1;

    return currentHeight;
}

int Cargo::currentTopHeight(int x, int y) const {
    if (!validateXY(x, y))
        return -1;

    const Containers &xyContainers = containers[x][y];
    auto height = this->shipPlan.getHeights()[x][y] + xyContainers.size();
    return height;
}

OptionalContainer Cargo::getTopContainer(int x, int y) const {
    if (!validateXY(x, y))
        return {};
    Containers xyContainers = containers[x][y];
    if (xyContainers.empty())
        return {};

    return xyContainers.back();
}

std::vector<ContainerPosition> Cargo::getContainersForPort(const PortId &portId) const {
    std::vector<ContainerPosition> result = std::vector<ContainerPosition>();

    Dimensions dims = shipPlan.getDimensions();
    for (int x = 0; x < dims.X(); x++)
        for (int y = 0; y < dims.Y(); y++) {
            Containers xyContainers = containers[x][y];

            for (int z = xyContainers.size() - 1; z >= 0; z--)
                if (xyContainers[z].getDestPort() == portId) {
                    auto height = this->shipPlan.getHeights()[x][y] + z;

                    Container cont(xyContainers[z]);

                    // TODO: a temporary value is passed to the constructor by reference, and all instances returned are the same one (??)
                    result.push_back(ContainerPosition(cont, {x, y, height}));
                }
        }

    return result;
}

OptionalContainer Cargo::getContainerById(std::string containerId) {
    auto iter = containersMapping.find(containerId);
    if (iter == containersMapping.end()) {
        return std::nullopt;
    }

    return iter->second;
}

int Cargo::numberOfEmptyPositions() {
    const Dimensions &dims = shipPlan.getDimensions();
    int sum = 0;
    for (int x = 0; x < dims.X(); x++)
        for (int y = 0; y < dims.Y(); y++) {
            Containers xyContainers = containers[x][y];
            sum += (dims.Z() - shipPlan.getHeights()[x][y]) - xyContainers.size();
        }

    return sum;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const Cargo &cargo) {
    os << "<CargoPrintNotImplemented>" << std::endl;
    return os;
    os << *cargo.getTopContainer(1, 1);
}

// endregion