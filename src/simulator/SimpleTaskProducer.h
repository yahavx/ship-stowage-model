//
// Created by Orr on 6/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SIMPLETASKPRODUCER_H
#define SHIP_STOWAGE_MODEL_SIMPLETASKPRODUCER_H

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<optional>
#include<functional>
#include<chrono>

#include "../common/utils/strongTypes.h"
#include "AlgorithmTravelTask.h"

// SimpleTasksProducer - you may want to separate it into .h and .cpp
class SimpleTasksProducer {
    std::vector<AlgorithmTravelTask> &tasks;
    const int numTasks = -1;
    const int iterationsPerTask = -1;
    std::atomic_int task_counter = 0;
    std::mutex m;

    std::optional<int> next_task_index();

    std::optional<int> next_task_index_simple();

public:
    SimpleTasksProducer(std::vector<AlgorithmTravelTask> &tasks);

    SimpleTasksProducer(SimpleTasksProducer &&other);

    std::optional <AlgorithmTravelTask> getTask();
};

#endif //SHIP_STOWAGE_MODEL_SIMPLETASKPRODUCER_H
