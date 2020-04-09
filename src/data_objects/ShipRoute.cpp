//
// Created by yahav on 05/04/2020.
//

#include "ShipRoute.h"

const std::vector<PortId> &ShipRoute::getPorts() const {
    return ports;
}

void ShipRoute::setPorts(const std::vector<PortId> &ports) {
    ShipRoute::ports = ports;
}

ShipRoute::ShipRoute(const std::vector<PortId> &ports) : ports(ports) {}

std::ostream &operator<<(std::ostream &os, const ShipRoute &route) {
    os << "ports: " << route.ports;
    return os;
}
