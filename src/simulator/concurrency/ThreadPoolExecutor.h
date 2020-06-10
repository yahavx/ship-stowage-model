//
// Created by Orr on 6/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_THREADPOOLEXECUTOR_H
#define SHIP_STOWAGE_MODEL_THREADPOOLEXECUTOR_H

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<optional>
#include<functional>
#include<chrono>

#include "../../common/utils/StrongTypes.h"


template<typename Producer>
/**
 * Consumes the given producer until exhausted or until manually stopped, using the specified number of threads.
 *
 * @tparam Producer
 */
class ThreadPoolExecutor {

    // region Attributes

    Producer producer;
    std::mutex printLock;
    const int numThreads = -1;
    std::vector<std::thread> workers;
    std::atomic_bool running = false;
    std::atomic_bool stopped = false;
    static thread_local int num_tasks_finished;
    std::atomic_int total_num_tasks_finished { 0 };

    // endregion

    // region Internal

    /**
     * Consumes a task from the producer, then execute it.
     * This function relies on the task having run() function.
     */
    void worker_function() {
        while(!stopped) {
            auto task = producer.getTask();
            if(!task) break;
            (*task).run();
            ++num_tasks_finished;
            ++total_num_tasks_finished;
        }

        const std::lock_guard<std::mutex> lock(printLock);
        if(stopped) {
            std::cout << std::this_thread::get_id() << " - stopped gracefully after processing " << num_tasks_finished << " task(s)" << std::endl;
        }
        else {
            std::cout << std::this_thread::get_id() << " - finished after processing " << num_tasks_finished << " task(s)" << std::endl;
        }
    }

    // endregion
public:

    // region Constructors

    ThreadPoolExecutor(Producer producer, NumThreads numThreads)
            : producer(std::move(producer)), numThreads(numThreads) {
        workers.reserve(numThreads);
    }

    // endregion

    // region Api

    /**
     * Starts numThreads number of worker threads, that consumes the producer.
     * @return
     */
    bool start() {
        bool running_status = false;
        if(!running.compare_exchange_strong(running_status, true)) {
            return false;
        }
        for(int i=0; i<numThreads; ++i) {
            workers.push_back(std::thread([this]{
                worker_function();
            }));
        }
        return true;
    }

    /**
     * Stop worker threads from taking new tasks, but waiting for all currently busy worker threads to finish before return.
     */
    void stop_gracefully() {
        stopped = true;
        wait_till_finish();
    }

    /**
     * Wait until all worker threads are done, then return.
     */
    void wait_till_finish() {
        for(auto& t : workers) {
            t.join();
        }

        const std::lock_guard<std::mutex> lock(printLock);
        std::cout << "thread pool finished/stopped after processing " << total_num_tasks_finished << " task(s)" << std::endl;
    }

    // endregion
};

template<typename Producer>
thread_local int ThreadPoolExecutor<Producer>::num_tasks_finished {0 };
#endif //SHIP_STOWAGE_MODEL_THREADPOOLEXECUTOR_H
