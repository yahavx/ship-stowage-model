cmake_minimum_required(VERSION 3.15)
project(ship_stowage_model)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

# Keep this order when you add a file

add_executable(ship_stowage_model
    # src
        # algorithms
        src/algorithms/_305170987_a.cpp src/algorithms/_305170987_a.h
        src/algorithms/_305170987_b.cpp src/algorithms/_305170987_b.h
        src/algorithms/BadAlgorithm.cpp src/algorithms/BadAlgorithm.h
        src/algorithms/NaiveStowageAlgorithm.cpp src/algorithms/NaiveStowageAlgorithm.h
        src/algorithms/RobustStowageAlgorithm.cpp src/algorithms/RobustStowageAlgorithm.h
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
            src/common/data_objects/Dimensions.cpp src/common/data_objects/Dimensions.h
            src/common/data_objects/Operations.cpp src/common/data_objects/Operations.h
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

            # loggers
            src/common/loggers/Tracer.cpp src/common/loggers/Tracer.h

            # strategies
            src/common/strategies/LoadContainerStrategy.h
            src/common/strategies/LoadContainerToArbitraryPosition.cpp
            src/common/strategies/LoadContainerToArbitraryPosition.h
            src/common/strategies/LoadContainerToLowestPositionAvailable.cpp
            src/common/strategies/LoadContainerToLowestPositionAvailable.h

            # utils
            src/common/utils/Definitions.h
            src/common/utils/Error.cpp src/common/utils/Error.h
            src/common/utils/Errors.cpp src/common/utils/Errors.h
            src/common/utils/Printers.cpp src/common/utils/Printers.h
            src/common/utils/StrongTypes.h
            src/common/utils/UtilFunctions.cpp src/common/utils/UtilFunctions.h
            src/common/utils/Vector3.cpp src/common/utils/Vector3.h


        # interfaces
            src/interfaces/AbstractAlgorithm.h
            src/interfaces/AlgorithmRegistration.h src/simulator/dynamic-load/AlgorithmRegistration.cpp
            src/interfaces/WeightBalanceCalculator.h src/interfaces/WeightBalanceCalculator.cpp

        # simulator
            # concurrency
            src/simulator/concurrency/AlgorithmTravelTask.cpp src/simulator/concurrency/AlgorithmTravelTask.h
            src/simulator/concurrency/AlgorithmTravelTaskProducer.cpp src/simulator/concurrency/AlgorithmTravelTaskProducer.h
            src/simulator/concurrency/ThreadPoolExecutor.h
            # dynamic-load
            src/simulator/dynamic-load/AlgorithmRegistrar.cpp src/simulator/dynamic-load/AlgorithmRegistrar.h
        
        src/simulator/AlgorithmValidation.cpp src/simulator/AlgorithmValidation.h
        src/simulator/SimulationManager.cpp src/simulator/SimulationManager.h
        src/simulator/main.cpp
        src/simulator/Simulator.cpp src/simulator/Simulator.h
        src/simulator/SimulatorFileManager.cpp src/simulator/SimulatorFileManager.h
        src/simulator/SimulatorUtil.cpp src/simulator/SimulatorUtil.h
        src/simulator/Travel.cpp src/simulator/Travel.h

    # tests
    tests/FileTests.h
    tests/IOTests.h
    tests/SimulationTests.h)