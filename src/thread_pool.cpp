#include "../include/thread_pool.h"
#include <stdexcept>


ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    // Create worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                {
                    // Use unique lock to wait for tasks
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    
                    // Wait until stop or task available
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });

                    // Exit thread if stopped and no tasks
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    // Get task from queue
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                // Execute the task
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    // Wake up all threads
    condition.notify_all();

    // Join all threads
    for (std::thread& worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        
        // Prevent enqueueing after stopping the pool
        if (stop) {
            throw std::runtime_error("Cannot enqueue on stopped thread pool");
        }

        tasks.emplace(task);
    }

    // Notify one waiting thread
    condition.notify_one();
}