//
// Created by yahav on 05/04/2020.
//

#include "ShipRoute.h"


// Constructors
ShipRoute::ShipRoute() {}

ShipRoute::ShipRoute(const std::vector<PortId> &ports) : ports(ports) {}


// Setters and getters
std::vector<PortId> &ShipRoute::getPorts() {
    return ports;
}

void ShipRoute::setPorts(std::vector<PortId> &ports) {
    ShipRoute::ports = ports;
}


// Printer
std::ostream &operator<<(std::ostream &os, const ShipRoute &route) {
    std::cout << "ShipRoute {" << std::endl;
    for (auto port : route.ports)
        std::cout << "\t" << port << std::endl;
    std::cout << "}" << std::endl;
    return os;
}
