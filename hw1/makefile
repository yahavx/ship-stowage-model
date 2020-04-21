COMP = g++-8.3.0

# put all your object files here
OBJS = main.o $\
$\
src/actors/ContainerShip.o src/actors/Port.o $\
$\
src/algorithms/stowage/NaiveStowageAlgorithm.o $\
src/algorithms/CranesOperation.o src/algorithms/WeightBalanceCalculator.o $\
$\
src/common/io/FileReader.o common/io/ObjectsReader $\
src/common/Constants.o $\
$\
src/data_objects/Container.o src/data_objects/ContainerPosition.o src/data_objects/PackingOperation.o src/data_objects/PortId.o $\
src/data_objects/Position.o src/data_objects/ShipPlan.o src/data_objects/ShipRoute.o $\
$\
src/data_structures/Cargo.o src/data_structures/ContainerStroage.o $\
$\
src/simulator/Simulator.o src/simulator/SimulatorUtil.o $\
$\
src/utils/Printers.o src/utils/UtilFunctions.o

# The executable filename
EXEC = ex1
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -I./src
CPP_LINK_FLAG = -lstdc++fs

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
# a rule for building a simple c++ source file

clean:
	rm -f $(OBJS) $(EXEC)