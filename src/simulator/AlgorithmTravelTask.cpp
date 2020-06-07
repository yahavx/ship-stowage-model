//
// Created by Orr on 6/7/2020.
//

#include "AlgorithmTravelTask.h"
#include "SimulationManager.h"
#include "../common/utils/UtilFunctions.h"
#include "SimulatorUtil.h"

#include <utility>

AlgorithmTravelTask::AlgorithmTravelTask(SimulatorFileManager fileManager, Tracer &tracer,
                                         StringStringVector &resultsTable, std::pair<longUInt, longUInt> resultsTableIndices,
                                         std::function<std::unique_ptr<AbstractAlgorithm>()> &algorithmFactory, std::string &travel)
        : fileManager(std::move(fileManager)), tracer(tracer),
          resultsTable(resultsTable), resultsTableIndices(std::move(resultsTableIndices)),
          algorithmFactory(algorithmFactory), travel(travel) {}

void AlgorithmTravelTask::run() {
    SimulationManager simManager(fileManager, tracer);

    // region Init

    tracer.traceInfo("Starting simulation (Algorithm = " + fileManager.algorithmName + ", Travel = " + fileManager.travelName + ")", true);

    WeightBalanceCalculator weightBalancer, algoWeightBalancer;

    simManager.initSimulationShip(weightBalancer);

    auto algorithm = algorithmFactory();

    bool success = simManager.initAlgorithmShip(algorithm.get(), algoWeightBalancer);
    simManager.initCargoData();  // order the files for each port, filter irrelevant files

    if (!success) {  // Algorithm failed to initialize
        tracer.traceInfo("Algorithm failed to initialize, terminating.");
        tracer.separator(TraceVerbosity::Info, 0, 3);
        simManager.saveErrors();
        return /*-1*/;
    }

    // endregion

    tracer.traceInfo("The ship has started its journey!", true);

    for (auto &portId : simManager.getRoutePorts()) {  // Start the journey
        tracer.traceInfo("The ship has docked at port " + portId.getCode());

        std::string instructionsOutputPath = simManager.getInstructionsForCargo(algorithm.get());
        success = simManager.performPackingOperations(instructionsOutputPath);

        if (!success) {
            simManager.saveErrors();
            return /*-1*/;
        }

        std::string message = simManager.isRouteFinished() ? "The ship is going into maintenance..." : "The ship is continuing to the next port...";
        tracer.traceInfo(message, true);
    }

    int totalNumberOfOps = simManager.finishSimulation();

    // Update the result in the result table
    updateSimulationResultAtPosition(
            resultsTable, totalNumberOfOps,
            resultsTableIndices.first + 1, resultsTableIndices.second + 1);

    tracer.traceInfo("The ship has completed its journey. Total number of operations: " + intToStr(totalNumberOfOps));
    tracer.separator(TraceVerbosity::Info, 0, 3);

    /*return totalNumberOfOps*/;
}