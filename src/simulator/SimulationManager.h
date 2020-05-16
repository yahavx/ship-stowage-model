//
// Created by t-yabeny on 5/16/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATIONMANAGER_H
#define SHIP_STOWAGE_MODEL_SIMULATIONMANAGER_H


#include "SimulatorFileManager.h"
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "../common/actors/CranesManagement.h"


/// Manages a single simulation (algorithm-travel pair).
class SimulationManager {
    SimulatorFileManager &fileManager;
    Errors errors;
    ContainerShip ship;
    int totalNumberOfOps;

    // internal
    Port currentPort;  // this will hold the current port (with storage) in each visit
    int portsVisited = 0;  // since the start of the journey
    StringToStringVectorMap cargoData;  // cargo_data files of each port
    StringToIntMap portsVisits;  // number of visits in each port

public:

    // region Constructor

    explicit SimulationManager(SimulatorFileManager &manager);

    // endregion

    // region Init

    /// Inits the ship of a single simulation. Assume no fatal errors are in the plan and route.
    void initSimulationShip(WeightBalanceCalculator &calculator);

    /**
     * Inits the algorithm in a single simulation. Returns the error return value of the algorithm.
     *
     * @return true if algorithm initialized successfully (according to returned error code).
     */
    int initAlgorithmShip(AbstractAlgorithm *algorithm, WeightBalanceCalculator &calculator);

    void initCargoData();

private:

    /// Inits a map from each port id to 0 (to be incremented each time we visit this port).
    void initPortsVisits(ShipRoute &shipRoute);

    /// Removes from map ports that doesn't appear in the route.
    void filterUnusedPorts(StringToStringVectorMap &map, ShipRoute &shipRoute);

    // endregion

    // region Simulation

public:

    std::vector<PortId> getRoutePorts();

    /**
     * Returns the visit number at the specified port. Port must exist in portsVisits - otherwise might throw an exception.
     *
     * @param increment increment the visit number before returning.
     */

    int currentPortVisitNum(bool increment = false);

    /// Returns true if the current port is last in the route.
    bool isCurrentLastPort();

    /// Returns the next .cargo_data file of the port with portId. Generates one if needed (inside tempDir). Also initializes port for the simulator.
    std::string getNextFileForPort();

    /// Inits the simulator port.
    void initPort(const std::string& cargoDataPath);

    /**
    * Perform packing operations received from algorithm, on simulator's ship. Validates each operation is legal.
     *
    * @param operationsPath path to read a list of operations from (output path of the algorithm).
    * @return true if no algorithm errors were encountered.
    */
    bool performPackingOperations(const std::string &operationsPath);

private:

    /// Checks the crane result is okay, adds an error if needed.
    void checkCraneResult(const PackingOperation &op, CraneOperationResult opResult);

    // endregion

    // region Finish

public:

    /// Reports to errors file that an algorithm error occurred in the simulation.
    void reportSimulationError();

    /// Finalize simulation, and return total number of steps (or -1).
    int finishSimulation();

    /// Saves error to a file, if there's at least one error.
    void saveErrors();

private:

    void validateNoCargoFilesLeft();

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATIONMANAGER_H
