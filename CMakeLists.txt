cmake_minimum_required(VERSION 3.15)
project(ship_stowage_model)

set(CMAKE_CXX_STANDARD 14)

add_executable(ship_stowage_model main.cpp src/Container.cpp src/Container.h src/ContainerShip.cpp src/ContainerShip.h src/ContainerPlan.cpp src/ContainerPlan.h src/PackingOperation.cpp src/PackingOperation.h src/ShipRoute.cpp src/ShipRoute.h src/WeightBalancer.h src/Port.cpp src/Port.h src/CranesOperation.h src/StowageProcess.h src/InputOutput.cpp src/InputOutput.h src/StowageProcess.cpp src/WeightBalancer.cpp src/CranesOperation.cpp)