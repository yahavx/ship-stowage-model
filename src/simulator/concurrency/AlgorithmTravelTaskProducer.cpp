//
// Created by Orr on 6/7/2020.
//

#include "AlgorithmTravelTaskProducer.h"

// region Constructors

AlgorithmTravelTaskProducer::AlgorithmTravelTaskProducer(const std::vector<AlgorithmTravelTask> &tasks)
        : tasks(tasks), numTasks(tasks.size()) {}

AlgorithmTravelTaskProducer::AlgorithmTravelTaskProducer(AlgorithmTravelTaskProducer &&other)
        : tasks(other.tasks), numTasks(other.numTasks), task_counter(other.task_counter.load()) {}

// endregion

// region Internal

std::optional<int> AlgorithmTravelTaskProducer::next_task_index() {
    for (int curr_counter = task_counter.load(); curr_counter < numTasks;) {
        if (task_counter.compare_exchange_weak(curr_counter, curr_counter + 1)) {
            return {curr_counter}; // zero based
        }
    }
    return {};
}

// endregion

// region Api

std::optional<AlgorithmTravelTask> AlgorithmTravelTaskProducer::getTask() {
    auto task_index = next_task_index();
    if (task_index) {
        AlgorithmTravelTask task = tasks[task_index.value()];
        return task;

    } else return {};
}

// endregion
