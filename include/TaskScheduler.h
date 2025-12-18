#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "Task.h"
#include "ThreadPool.h"
#include <queue>
#include <memory>
#include <mutex>
#include <map>
#include <atomic>

class TaskScheduler {
public:
    explicit TaskScheduler(size_t num_threads = std::thread::hardware_concurrency());
    ~TaskScheduler();

    Task::TaskId addTask(Task::TaskFunction func, 
                         TaskPriority priority = TaskPriority::NORMAL,
                         const std::string& name = "");

    void start();
    void stop();
    void waitForAll();
    
    size_t getPendingTaskCount() const;
    size_t getCompletedTaskCount() const;

private:
    std::unique_ptr<ThreadPool> thread_pool_;
    
    std::priority_queue<std::shared_ptr<Task>> task_queue_;
    std::map<Task::TaskId, std::shared_ptr<Task>> tasks_;
    
    mutable std::mutex queue_mutex_;
    std::atomic<bool> running_;
    std::atomic<Task::TaskId> next_task_id_;
    std::atomic<size_t> completed_tasks_;
    
    std::thread scheduler_thread_;
    
    void schedulerLoop();
    void executeTask(std::shared_ptr<Task> task);
};

#endif