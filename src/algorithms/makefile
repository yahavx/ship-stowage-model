COMP = g++-8.3.0

SHARED_OBJS = $(NAIVE_SO) $(ROBUST_SO)
NAIVE_SO = _305170987_a.so
ROBUST_SO = _305170987_b.so
NAIVE = _305170987_a.o
ROBUST = _305170987_b.o

OBJS = BadAlgorithm.o SemiAbstractAlgorithm.o NaiveStowageAlgorithm.o RobustStowageAlgorithm.o $\
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
../common/strategies/LoadContainerToArbitraryPosition.o ../common/strategies/LoadContainerToLowestPositionAvailable.o $\
$\
../common/utils/Error.o ../common/utils/Errors.o $\
../common/utils/Printers.o ../common/utils/UtilFunctions.o ../common/utils/Vector3.o # $\
# $\
# ../interfaces/WeightBalanceCalculator.o

CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS)

$(NAIVE_SO): $(OBJS) $(NAIVE)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

$(ROBUST_SO): $(OBJS) $(ROBUST)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^
	
$(OBJS) $(ROBUST) $(NAIVE):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@
	
clean:
	rm -f $(OBJS) $(SHARED_OBJS)
