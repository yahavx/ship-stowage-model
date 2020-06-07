//
// Created by Orr on 6/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASKPRODUCER_H
#define SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASKPRODUCER_H

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<optional>
#include<functional>
#include<chrono>

#include "../../common/utils/StrongTypes.h"
#include "AlgorithmTravelTask.h"

/**
 * Producer for Algorithm-Travel tasks.
 * Produces all tasks passed to the constructor.
 *
 * ThreadPoolExecutor is meant to consume this producer.
 */
class AlgorithmTravelTaskProducer {
    const std::vector<AlgorithmTravelTask> &tasks;
    const int numTasks = -1;
    std::atomic_int task_counter = 0;
    std::mutex m;

    // region Internal

    std::optional<int> next_task_index();

    // endregion

public:

    // region Constructors

    AlgorithmTravelTaskProducer(const std::vector<AlgorithmTravelTask> &tasks);

    AlgorithmTravelTaskProducer(AlgorithmTravelTaskProducer &&other);

    // endregion

    // region Api

    /**
     * Produces task if there are available tasks
     */
    std::optional<AlgorithmTravelTask> getTask();

    // endregion
};

#endif //SHIP_STOWAGE_MODEL_ALGORITHMTRAVELTASKPRODUCER_H
