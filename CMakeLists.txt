cmake_minimum_required(VERSION 3.15)
project(ship_stowage_model)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

# Keep this order when you add a file

add_executable(ship_stowage_model
    main.cpp
    # src
        # algorithms
        src/algorithms/BadAlgorithm.cpp src/algorithms/BadAlgorithm.h
        src/algorithms/NaiveStowageAlgorithm.cpp src/algorithms/NaiveStowageAlgorithm.h
        src/algorithms/SemiAbstractAlgorithm.cpp src/algorithms/SemiAbstractAlgorithm.h

        # common
            # actors
            src/common/actors/AbstractWeightBalancer.cpp src/common/actors/AbstractWeightBalancer.h
            src/common/actors/ContainerShip.cpp src/common/actors/ContainerShip.h
        src/common/actors/CranesManagement.cpp src/common/actors/CranesManagement.h
            src/common/actors/NaiveWeightBalancer.h src/common/actors/NaiveWeightBalancer.cpp
            src/common/actors/Port.cpp src/common/actors/Port.h

        # data_objects
            src/common/data_objects/Container.cpp src/common/data_objects/Container.h
            src/common/data_objects/ContainerPosition.cpp src/common/data_objects/ContainerPosition.h
            src/common/data_objects/PackingOperation.cpp src/common/data_objects/PackingOperation.h
            src/common/data_objects/PortId.cpp src/common/data_objects/PortId.h
            src/common/data_objects/Position.cpp src/common/data_objects/Position.h
            src/common/data_objects/ShipPlan.cpp src/common/data_objects/ShipPlan.h
            src/common/data_objects/ShipRoute.cpp src/common/data_objects/ShipRoute.h

            # data_structures
            src/common/data_structures/Cargo.cpp src/common/data_structures/Cargo.h
            src/common/data_structures/ContainerStorage.cpp src/common/data_structures/ContainerStorage.h

            # io
            src/common/io/FileReader.cpp src/common/io/FileReader.h
            src/common/io/ObjectsReader.cpp src/common/io/ObjectsReader.h

            # utils
            src/common/utils/Constants.cpp src/common/utils/Constants.h
            src/common/utils/Definitions.h
            src/common/utils/Errors.cpp src/common/utils/Errors.h
            src/common/utils/Printers.cpp src/common/utils/Printers.h
            src/common/utils/UtilFunctions.cpp src/common/utils/UtilFunctions.h

        # interfaces
        src/interfaces/AbstractAlgorithm.h
        src/interfaces/AlgorithmRegistration.h src/interfaces/AlgorithmRegistration.cpp
        src/interfaces/WeightBalanceCalculator.h src/interfaces/WeightBalanceCalculator.cpp

        # simulator
        src/simulator/AlgorithmValidation.cpp src/simulator/AlgorithmValidation.h
        src/simulator/Simulator.cpp src/simulator/Simulator.h
        src/simulator/SimulatorDataManager.cpp src/simulator/SimulatorDataManager.h
        src/simulator/SimulatorUtil.cpp src/simulator/SimulatorUtil.h

    # tests
    tests/FileTests.h
    tests/IOTests.h
    tests/SimulationTests.h)