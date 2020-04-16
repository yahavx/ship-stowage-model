//
// Created by Orr on 4/9/2020.
//

#include "StowageAlgorithmInterface.h"

StowageAlgorithmInterface::StowageAlgorithmInterface(ContainerShip &ship, std::map<PortId, Port> &ports) : ship(ship), ports(ports) {}
