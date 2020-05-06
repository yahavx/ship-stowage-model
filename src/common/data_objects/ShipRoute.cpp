//
// Created by yahav on 05/04/2020.
//

#include "ShipRoute.h"


// region Constructors

ShipRoute::ShipRoute() {}

ShipRoute::ShipRoute(const std::vector<PortId> &ports) : ports(ports) {}

// endregion

// region  Setters and getters

std::vector<PortId> &ShipRoute::getPorts() {
    return ports;
}

void ShipRoute::setPorts(std::vector<PortId> &ports) {
    ShipRoute::ports = ports;
}

const PortId& ShipRoute::getFirstPort() const {
    return this->ports[0];
}

// endregion

// region Functions

std::unordered_set<std::string> ShipRoute::getNextPortsSet() const {
    std::unordered_set<std::string> portsSet;
    for (auto& portId: ports) {
        portsSet.insert(portId);
    }
    return portsSet;
}

// endregion

// region Printer

std::ostream &operator<<(std::ostream &os, const ShipRoute &route) {
    std::cout << "ShipRoute {" << std::endl;
    for (auto port : route.ports)
        std::cout << "\t" << port << std::endl;
    std::cout << "}" << std::endl;
    return os;
}

// endregion