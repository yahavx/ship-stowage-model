cmake_minimum_required(VERSION 3.15)
project(ship_stowage_model)

set(CMAKE_CXX_STANDARD 14)

add_executable(ship_stowage_model main.cpp src/Container.cpp src/Container.h src/ContainerShip.cpp src/ContainerShip.h src/ShipPlan.cpp src/ShipPlan.h src/ShipFloor.cpp src/ShipFloor.h src/FloorPlan.cpp src/FloorPlan.h src/PackingOperation.cpp src/PackingOperation.h src/ShipRoute.cpp src/ShipRoute.h src/IWeightBalancer.h src/Port.cpp src/Port.h)