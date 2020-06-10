//
// Created by t-yabeny on 4/18/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMULATOR_H
#define SHIP_STOWAGE_MODEL_SIMULATOR_H

#include "../interfaces/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "../common/utils/Errors.h"
#include "SimulatorFileManager.h"
#include "../common/data_objects/Operations.h"
#include "../common/loggers/Tracer.h"
#include "concurrency/AlgorithmTravelTask.h"
#include "concurrency/AlgorithmTravelTaskProducer.h"
#include <memory>
#include <functional>


class Simulator {
    std::string travelRootDir;
    std::string algorithmsDir;
    std::string outputDir;
    int numThreads;

    SimulatorFileManager rootFileManager;
    std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
    StringVector algorithmNames;  // the entry i corresponds to the name of the algorithm of factory i

    Tracer tracer = Tracer(TraceVerbosity::Disabled);
//    Tracer tracer = Tracer(TraceVerbosity::Info);
//    Tracer tracer = Tracer(TraceVerbosity::Verbose);

    Tracer threadsTracer = Tracer(TraceVerbosity::Info);

public:

    // region Constructors

    Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir, int numThreads);

    Simulator(const std::string &travelRootDir, int numThreads, const std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &algorithmFactories, const std::string &outputDir);  // for tests

    // endregion

    // region Simulation run

    /// Run a cartesian loop of “travel” X “algorithm”.
    void runSimulations();

private:

    /// Loads algorithms dynamically, returns false if no algorithm was loaded.
    void loadAlgorithmsDynamically(Errors &errors);

    /**
     * Creates list of all Algorithm-Travel pair tasks
     */
    std::vector<AlgorithmTravelTask> createAlgorithmTravelTasks(StringVector &travels, StringStringVector &resultsTable);

    /// Executes the travel x algorithms in a single thread.
    void executeSimulationsSingleThread(std::vector<AlgorithmTravelTask> & tasks);

    /// Executes the travel x algorithms with multi threading.
    void executeSimulationsMultiThread(std::vector<AlgorithmTravelTask> & tasks);

    // endregion

    // region Constants

public:

    static const std::string s_resultsTableTitle;
    static const std::string s_sumColumnTitle;
    static const std::string s_errorsColumnTitle;
    static const std::string s_resultsTablePlaceholder;


    // endregion
};


#endif //SHIP_STOWAGE_MODEL_SIMULATOR_H
