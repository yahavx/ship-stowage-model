COMP = g++-9.3.0

SHARED_OBJS = $(ALGO1_SO) $(ALGO2_SO)
ALGO1_SO = _305170987_a.so
ALGO2_SO = _305170987_b.so
ALGO1 = _305170987_a.o
ALGO2 = _305170987_b.o

ALGORITHM_OBJS = BadAlgorithm.o SemiAbstractAlgorithm.o NaiveStowageAlgorithm.o EfficientStowageAlgorithm.o RobustStowageAlgorithm.o

COMMON_OBJS = ../common/actors/AbstractWeightBalancer.o ../common/actors/ContainerShip.o ../common/actors/CranesManagement.o $\
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
../common/utils/Printers.o ../common/utils/UtilFunctions.o ../common/utils/Vector3.o

CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
CPP_LINK_FLAG = -shared

OBJS = $(ALGORITHM_OBJS) $(COMMON_OBJS)

all: clean $(SHARED_OBJS)

$(ALGO1_SO): $(OBJS) $(ALGO1)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

$(ALGO2_SO): $(OBJS) $(ALGO2)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

$(ALGORITHM_OBJS) $(COMMON_OBJS) $(ALGO2) $(ALGO1):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

clean:
	rm -f $(OBJS) $(COMMON_OBJS) $(SHARED_OBJS) $(ALGO1) $(ALGO2)
