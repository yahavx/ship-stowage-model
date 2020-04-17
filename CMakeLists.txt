cmake_minimum_required(VERSION 3.15)
project(ship_stowage_model)

set(CMAKE_CXX_STANDARD 14)

# Keep this order when you add a file

add_executable(ship_stowage_model
        main.cpp

        src/actors/ContainerShip.cpp src/actors/ContainerShip.h
        src/actors/Port.cpp src/actors/Port.h

        src/algorithms/stowage/NaiveStowageAlgorithm.cpp src/algorithms/stowage/NaiveStowageAlgorithm.h
        src/algorithms/stowage/StowageAlgorithmInterface.cpp src/algorithms/stowage/StowageAlgorithmInterface.h

        src/algorithms/CranesOperation.cpp src/algorithms/CranesOperation.h
        src/algorithms/WeightBalanceCalculator.cpp src/algorithms/WeightBalanceCalculator.h

        src/data_objects/Container.cpp src/data_objects/Container.h
        src/data_objects/ContainerPosition.cpp src/data_objects/ContainerPosition.h
        src/data_objects/PackingOperation.cpp src/data_objects/PackingOperation.h
        src/data_objects/PortId.cpp src/data_objects/PortId.h
        src/data_objects/ShipPlan.cpp src/data_objects/ShipPlan.h
        src/data_objects/ShipRoute.cpp src/data_objects/ShipRoute.h

        src/data_structures/Cargo.cpp src/data_structures/Cargo.h
        src/data_structures/ContainerStorage.cpp src/data_structures/ContainerStorage.h

        src/io/FileReader.cpp src/io/FileReader.h
        src/io/ObjectsReader.cpp src/io/ObjectsReader.h

        src/utils/Definitions.h
        src/utils/UtilFunctions.cpp src/utils/UtilFunctions.h)