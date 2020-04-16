//
// Created by yahav on 05/04/2020.
//

#include "ShipRoute.h"


// Constructors
ShipRoute::ShipRoute() {}

ShipRoute::ShipRoute(const std::vector<PortId> &ports) : ports(ports) {}


// Setters and getters
const std::vector<PortId> &ShipRoute::getPorts() const {
    return ports;
}

void ShipRoute::setPorts(const std::vector<PortId> &ports) {
    ShipRoute::ports = ports;
}


// Printer
std::ostream &operator<<(std::ostream &os, const ShipRoute &route) {
    std::cout << "ShipRoute(";
    auto ports = route.getPorts();
    for(int i = 0; i < ports.size() - 1; i++)
        std::cout << ports[i] << ", ";
    std::cout << ports.back() << ')';
    return os;
}
