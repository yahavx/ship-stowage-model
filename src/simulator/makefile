COMP = g++-8.3.0

OBJS = main.o AlgorithmValidation.o Simulator.o SimulatorDataManager.o SimulatorUtil.o $\
$\
../common/actors/AbstractWeightBalancer.o ../common/actors/ContainerShip.o ../common/actors/CranesManagement.o $\
../common/actors/NaiveWeightBalancer.o ../common/actors/Port.o $\
$\
../common/data_objects/Container.o ../common/data_objects/ContainerPosition.o ../common/data_objects/PackingOperation.o $\
../common/data_objects/PortId.o ../common/data_objects/Position.o ../common/data_objects/ShipPlan.o $\
../common/data_objects/ShipRoute.o $\
$\
../common/data_structures/Cargo.o ../common/data_structures/ContainerStorage.o $\
$\
../common/io/FileReader.o ../common/io/ObjectsReader.o $\
$\
../common/utils/Constants.o ../common/utils/Errors.o $\
../common/utils/Printers.o ../common/utils/UtilFunctions.o $\
$\
../interfaces/AlgorithmRegistration.o ../interfaces/WeightBalanceCalculator.o

EXEC = simulator
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -ldl -export-dynamic #(-rdynamic)

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@ 
	
# AlgorithmRegistrar.o: AlgorithmRegistrar.cpp #put dependencies here!
#	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

# AlgorithmRegistration.o: AlgorithmRegistration.cpp #put dependencies here!
#	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

main.o: main.cpp #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
	
clean:
	rm -f $(OBJS) $(EXEC)
