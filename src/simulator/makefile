COMP = g++-9.3.0

OBJS = main.o $\ AlgorithmValidation.o SimulationManager.o $\
Simulator.o SimulatorFileManager.o SimulatorUtil.o Travel.o $\
$\
./concurrency/AlgorithmTravelTask.o ./concurrency/AlgorithmTravelTaskProducer.o $\
$\
./dynamic-load/AlgorithmRegistrar.o ./dynamic-load/AlgorithmRegistration.o $\
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
../common/strategies/LoadContainerInGroups.o $\
$\
../common/utils/Error.o ../common/utils/Errors.o $\
../common/utils/Printers.o ../common/utils/UtilFunctions.o ../common/utils/Vector3.o $\
$\
../interfaces/WeightBalanceCalculator.o

EXEC = simulator
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -ldl -export-dynamic -lstdc++fs -pthread #(-rdynamic)

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

clean:
	rm -f $(OBJS) $(EXEC)

# ./simulator -travel_path ../Travel-Roots/multiple-travel -algorithm_path ../algorithms -output ../output