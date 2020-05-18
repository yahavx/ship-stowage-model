COMP = g++-8.3.0

OBJS = main.o AlgorithmRegistrar.o AlgorithmValidation.o SimulationManager.o $\
Simulator.o SimulatorFileManager.o SimulatorUtil.o $\
$\
../common/actors/AbstractWeightBalancer.o ../common/actors/ContainerShip.o ../common/actors/CranesManagement.o $\
../common/actors/NaiveWeightBalancer.o ../common/actors/Port.o $\
$\
../common/data_objects/Container.o ../common/data_objects/ContainerPosition.o ../common/data_objects/Dimensions.o $\
../common/data_objects/Operations.o ../common/data_objects/PackingOperation.o $\
../common/data_objects/PortId.o ../common/data_objects/Position.o ../common/data_objects/ShipPlan.o $\
../common/data_objects/ShipRoute.o $\
$\
../common/data_structures/Cargo.o ../common/data_structures/ContainerStorage.o $\
$\
../common/io/FileReader.o ../common/io/ObjectsReader.o $\
$\
../common/loggers/Tracer.o $\
$\
../common/strategies/LoadContainerToArbitraryPosition.o ../common/strategies/LoadContainerToLowestPositionAvailable.o $\
$\
../common/utils/Error.o ../common/utils/Errors.o $\
../common/utils/Printers.o ../common/utils/UtilFunctions.o ../common/utils/Vector3.o $\
$\
../interfaces/AlgorithmRegistration.o ../interfaces/WeightBalanceCalculator.o

EXEC = simulator
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -ldl -export-dynamic -lstdc++fs #(-rdynamic)

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

# AlgorithmRegistrar.o: AlgorithmRegistrar.cpp #put dependencies here!
#	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

# AlgorithmRegistration.o: AlgorithmRegistration.cpp #put dependencies here!
#	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


clean:
	rm -f $(OBJS) $(EXEC)

# ./simulator -travel_path ../test -algorithm_path ../algorithms -output ../output

# valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./simulator -travel_path ../input-examples/single-travel -algorithm_path ../algorithms -output ../output
