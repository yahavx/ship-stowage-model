//
// Created by t-yabeny on 4/18/2020.
//

#include <iostream>
#include "Simulator.h"
#include "SimulatorUtil.h"
#include "AlgorithmValidation.h"
#include "../common/io/ObjectsReader.h"
#include "../common/utils/Printers.h"
#include "../common/actors/CranesManagement.h"
#include "../common/utils/Errors.h"
#include "../common/io/FileReader.h"
#include "../common/utils/UtilFunctions.h"
#include "../interfaces/AbstractAlgorithm.h"
#include "dynamic-load/AlgorithmRegistrar.h"
#include "SimulationManager.h"

#ifndef RUNNING_ON_NOVA

#include "../algorithms/NaiveStowageAlgorithm.h"
#include "../algorithms/BadAlgorithm.h"
#include "../algorithms/RobustStowageAlgorithm.h"
#include "concurrency/AlgorithmTravelTaskProducer.h"
#include "../common/utils/StrongTypes.h"
#include "concurrency/AlgorithmTravelTask.h"
#include "concurrency/ThreadPoolExecutor.h"

#endif

// region Constructors

Simulator::Simulator(const std::string &travelRootDir, const std::string &algorithmsDir, const std::string &outputDir, const int numThreads) : travelRootDir(
        travelRootDir),
                                                                                                                                               algorithmsDir(
                                                                                                                                                       algorithmsDir),
                                                                                                                                               outputDir(
                                                                                                                                                       outputDir),
                                                                                                                                               numThreads(
                                                                                                                                                       numThreads),
                                                                                                                                               rootFileManager(
                                                                                                                                                       outputDir,
                                                                                                                                                       travelRootDir) {
#ifndef RUNNING_ON_NOVA
    algorithmFactories.emplace_back([]() { return std::make_unique<NaiveStowageAlgorithm>(); });
    algorithmNames.push_back("Naive");

    algorithmFactories.emplace_back([](){return std::make_unique<BadAlgorithm>();});
    algorithmNames.push_back("Bad");
//
    algorithmFactories.emplace_back([]() { return std::make_unique<RobustStowageAlgorithm>(); });
    algorithmNames.push_back("Robust");
#endif
}

Simulator::Simulator(const std::string &travelRootDir, int numThreads,
                     const std::vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &algorithmFactories,
                     const std::string &outputDir) : travelRootDir(travelRootDir), outputDir(outputDir), numThreads(numThreads),
                                                     rootFileManager(outputDir, travelRootDir),
                                                     algorithmFactories(algorithmFactories) {}


// endregion

// region Simulation run

void Simulator::runSimulations() {
    tracer.traceVerbose("Simulator started.", true);
    StringStringVector resultsTable;  // table of results
    Errors generalErrors;

    // load algorithms, create output folder
    std::vector<Travel> travels = rootFileManager.collectLegalTravels(generalErrors);
    rootFileManager.createOutputFolders(generalErrors);
    loadAlgorithmsDynamically(generalErrors);  // We may have no travels to run at this point - but we can collect errors
    generalErrors.addSimulatorInitLog();

    // init data table
    initResultsTableWithPlaceholders(resultsTable, travels, algorithmNames);  // Add columns names and set table structure

    auto tasks = createAlgorithmTravelTasks(travels, resultsTable);

    // execute travels x algorithms
    numThreads == 1 ? executeSimulationsSingleThread(tasks) : executeSimulationsMultiThread(tasks);

    // save data table
    if (!travels.empty() && !algorithmFactories.empty()) {
        finalizeResultsTable(resultsTable);
        rootFileManager.saveSimulationResults(resultsTable);
    } else {
        tracer.traceInfo("No legal travels and/or no algorithms were available (no simulations were ran).");
    }

    // save errors
    if (generalErrors.hasErrors()) {
        rootFileManager.saveGeneralErrors(generalErrors);
    }

    rootFileManager.cleanOutputFolders();  // remove temp and errors (if empty)
}

void Simulator::loadAlgorithmsDynamically(Errors &errors) {
#ifndef RUNNING_ON_NOVA
    return;
#endif
    tracer.traceVerbose("Looking for algorithms...", true);

    if (!isDirectoryExists(algorithmsDir)) {
        errors.addError({ErrorFlag::SharedObject_InvalidDirectory, algorithmsDir});
        return;
    }

    if (isFolderEmpty(algorithmsDir)) {
        errors.addError({ErrorFlag::SharedObject_InvalidDirectory, algorithmsDir});
        return;
    }

    auto files = getFilesFromDirectory(algorithmsDir);

    auto &registrar = AlgorithmRegistrar::getInstance();

    for (auto &file: files) {
        if (!endsWith(file, ".so")) {
            continue;
        }

        tracer.traceVerbose("Loading file: " + file);

        ErrorFlag soLoadResult = registrar.loadSharedObject(file);
        if (soLoadResult != ErrorFlag::Success) {
            errors.addError({soLoadResult, extractFilenameFromPath(file)});
            continue;
        }

        int delta = registrar.factoriesIncrease();  // how many algorithms were added

        switch (delta) {
            case 0:
                errors.addError({ErrorFlag::SharedObject_AlgorithmDidntSelfRegister, extractFilenameFromPath(file)});
                break;
            case 1:  // The expected behaviour
                algorithmFactories.push_back(registrar.getLast());
                algorithmNames.push_back(extractFilenameFromPath(file, true));
                break;
            default:
                errors.addError({ErrorFlag::SharedObject_LoadedMoreThanOneAlgorithm, extractFilenameFromPath(file)});
                break;
        }
    }

    tracer.separator(TraceVerbosity::Verbose, 0, 0);
    tracer.traceVerbose("Finished looking for algorithms.", true);

    if (algorithmFactories.empty()) {
        errors.addError(ErrorFlag::SharedObject_NoAlgorithmsLoaded);
    }
}

std::vector<AlgorithmTravelTask> Simulator::createAlgorithmTravelTasks(std::vector<Travel> &travels, StringStringVector &resultsTable) {
    std::vector<AlgorithmTravelTask> tasks;

    for (longUInt t = 0; t < travels.size(); t++) {
        auto &travel = travels[t];
        for (longUInt a = 0; a < algorithmFactories.size(); a++) {
            // create a task
            SimulatorFileManager fileManager(outputDir, travelRootDir, extractFilenameFromPath(travel), algorithmNames[a]);
            AlgorithmTravelTask task(fileManager, tracer, resultsTable, std::pair(a, t), algorithmFactories[a], travel);
            tasks.push_back(task);

            tracer.traceVerbose("Created task: <" + algorithmNames[a] + ", " + extractFilenameFromPath(travel) + ">");
        }
    }

    return tasks;
}

void Simulator::executeSimulationsSingleThread(std::vector<AlgorithmTravelTask> &tasks) {
    threadsTracer.traceInfo("Starting execution on a single thread", true);

    for (auto &task : tasks) {
        task.run();
    }

    threadsTracer.traceInfo("Finished execution on a single thread");
}

void Simulator::executeSimulationsMultiThread(std::vector<AlgorithmTravelTask> &tasks) {
    threadsTracer.traceInfo("Starting execution on a multi thread", true);

    ThreadPoolExecutor executor{
            // Create producer for all algorithm-travel pair tasks
            AlgorithmTravelTaskProducer(tasks),
            NumThreads{numThreads},
            threadsTracer
    };

    // Start running all the tasks
    executor.start();

    // Wait until all tasks are finished
    executor.wait_till_finish();
}

// endregion

// region Constants

const std::string Simulator::s_resultsTableTitle = "RESULTS";
const std::string Simulator::s_sumColumnTitle = "Sum";
const std::string Simulator::s_errorsColumnTitle = "Num Errors";
const std::string Simulator::s_resultsTablePlaceholder = "-1";


// endregion
