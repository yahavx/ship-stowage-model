cmake_minimum_required(VERSION 3.15)
project(ship_stowage_model)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

# Keep this order when you add a file

add_executable(ship_stowage_model
        main.cpp

        # actors
        src/actors/ContainerShip.cpp src/actors/ContainerShip.h
        src/actors/Port.cpp src/actors/Port.h

        # algorithms
            # stowage
            src/algorithms/stowage/NaiveStowageAlgorithm.cpp src/algorithms/stowage/NaiveStowageAlgorithm.h
            src/algorithms/stowage/IStowageAlgorithm.h
        src/algorithms/CranesOperation.cpp src/algorithms/CranesOperation.h
        src/algorithms/WeightBalanceCalculator.cpp src/algorithms/WeightBalanceCalculator.h

        # common
            # io
            src/common/io/FileReader.cpp src/common/io/FileReader.h
            src/common/io/ObjectsReader.cpp src/common/io/ObjectsReader.h
        src/common/Constants.cpp src/common/Constants.h

        # data_objects
        src/data_objects/Container.cpp src/data_objects/Container.h
        src/data_objects/ContainerPosition.cpp src/data_objects/ContainerPosition.h
        src/data_objects/PackingOperation.cpp src/data_objects/PackingOperation.h
        src/data_objects/PortId.cpp src/data_objects/PortId.h
        src/data_objects/Position.cpp src/data_objects/Position.h
        src/data_objects/ShipPlan.cpp src/data_objects/ShipPlan.h
        src/data_objects/ShipRoute.cpp src/data_objects/ShipRoute.h

        # data_structures
        src/data_structures/Cargo.cpp src/data_structures/Cargo.h
        src/data_structures/ContainerStorage.cpp src/data_structures/ContainerStorage.h

        # simulator
        src/simulator/Simulator.cpp src/simulator/Simulator.h
        src/simulator/SimulatorUtil.cpp src/simulator/SimulatorUtil.h

        # utils
        src/utils/Definitions.h
        src/utils/Printers.cpp src/utils/Printers.h
        src/utils/UtilFunctions.cpp src/utils/UtilFunctions.h

        # tests
        tests/IOTests.h
        tests/SimulationTests.h)