COMP = g++-8.3.0

SHARED_OBJS = NaiveStowageAlgorithm.so RobustStowageAlgorithm.so
ROBUST = RobustStowageAlgorithm.o
NAIVE = NaiveStowageAlgorithm.o 

OBJS = BadAlgorithm.o SemiAbstractAlgorithm.o $\
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
../interfaces/WeightBalanceCalculator.o

CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS)

NaiveStowageAlgorithm.so: $(OBJS) NaiveStowageAlgorithm.o 
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

RobustStowageAlgorithm.so: $(OBJS) RobustStowageAlgorithm.o 
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^
	
$(OBJS) RobustStowageAlgorithm.o NaiveStowageAlgorithm.o:
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@
	
clean:
	rm -f $(OBJS) $(SHARED_OBJS)
