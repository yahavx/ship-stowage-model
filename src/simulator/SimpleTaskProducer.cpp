//
// Created by Orr on 6/7/2020.
//

#include "SimpleTaskProducer.h"


SimpleTasksProducer::SimpleTasksProducer(std::vector<AlgorithmTravelTask> &tasks)
        : tasks(tasks), numTasks(tasks.size()), iterationsPerTask(1) {}

SimpleTasksProducer::SimpleTasksProducer(SimpleTasksProducer &&other)
        : tasks(other.tasks), numTasks(other.numTasks), iterationsPerTask(other.iterationsPerTask), task_counter(other.task_counter.load()) {}

std::optional<int> SimpleTasksProducer::next_task_index() {
    for (int curr_counter = task_counter.load(); curr_counter < numTasks;) {
        // see: https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
        // note that in case compare_exchange_weak fails because the value of
        // task_counter != curr_counter than task_counter.load() is copied into curr_counter
        // in case of spurious failure (value not checked) curr_counter would not change
        if (task_counter.compare_exchange_weak(curr_counter, curr_counter + 1)) {
            return {curr_counter}; // zero based
        }
    }
    return {};
}

std::optional<int> SimpleTasksProducer::next_task_index_simple() {
    // this is a more simple approach for getting the next task index
    // it will return each time an unused index in the range[0, numTasks)
    // the difference from the one above is that at certain point of time
    // the variable task_counter may exceed numTasks by any number between 1 and numThreads-1
    // however, we will not return such values
    if (task_counter < numTasks) {
        int next_counter = ++task_counter; // atomic operation
        // another thread could have increment task_counter after we passed the prev if
        // so we must check again - following check is on a local variable so no race
        if (next_counter <= numTasks) {
            return {next_counter - 1}; // zero based
        } else {
            // just so that at the end we will have: task_counter == numTasks
            // this is not critical in our scenario but might be in other scenarios
            --task_counter;
        }
    }
    return {};
}

std::optional<AlgorithmTravelTask> SimpleTasksProducer::getTask() {
    auto task_index = next_task_index(); // or: next_task_index_simple();
    if (task_index) {
        AlgorithmTravelTask task = tasks[task_index.value()];
        return task;
//        return [task_index, this] {
//            for (int i = 0; i < iterationsPerTask; ++i) {
//                std::lock_guard g{m};
//                std::cout << std::this_thread::get_id() << "-" << *task_index << ": " << i << std::endl;
//                std::this_thread::yield();
//            }
//        };
    } else return {};
}
