COMP = g++-8.3.0

SHARED_OBJS = NaiveStowageAlgorithm.so RobustStowageAlgorithm.so
OBJS = BadAlgorithm.o NaiveStowageAlgorithm.o RobustStowageAlgorithm.o SemiAbstractAlgorithm.o $\
$\
../common/actors/AbstractWeightBalancer.o ../common/actors/ContainerShip ../common/actors/CranesManagement $\
../common/actors/NaiveWeightBalancer.o ../common/actors/Port.o $\
$\
../common/data_objects/Container.o ../common/data_objects/ContainerPosition ../common/data_objects/PackingOperation $\
../common/data_objects/PortId.o ../common/data_objects/Position.o ../common/data_objects/ShipPlan.o $\
../common/data_objects/ShipRoute.o $\
$\
../common/data_structures/Cargo.o ../common/src/ContainerStorage.o ../common/io/FileReader.o ../common/io/ObjectsReader.o $\
$\
../common/utils/Constants.o ../common/utils/Definitions.o ../common/util/Errors.o $\
../common/utils/Printers.o ../common/utils/UtilFunctions.o $\
$\
../interfaces/AbstractAlgorithm.o ../interfaces/AlgorithmRegistration.o ../interfaces/WeightBalanceCalculator.o

CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS)

$(SHARED_OBJS): $(OBJS)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@
	
clean:
	rm -f $(OBJS) $(SHARED_OBJS)
