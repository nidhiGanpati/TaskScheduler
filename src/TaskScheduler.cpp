#include "TaskScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>

TaskScheduler::TaskScheduler(size_t num_threads)
    : thread_pool_(std::make_unique<ThreadPool>(num_threads)),
      running_(false), next_task_id_(0), completed_tasks_(0) {}

TaskScheduler::~TaskScheduler() {
    stop();
}

Task::TaskId TaskScheduler::addTask(Task::TaskFunction func, TaskPriority priority, const std::string& name) {
    Task::TaskId id = next_task_id_++;
    
    auto task = std::make_shared<Task>(id, func, 
        name.empty() ? "Task_" + std::to_string(id) : name, priority);
    
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        tasks_[id] = task;
        task_queue_.push(task);
    }
    
    return id;
}

void TaskScheduler::start() {
    if (running_) return;
    
    running_ = true;
    scheduler_thread_ = std::thread(&TaskScheduler::schedulerLoop, this);
}

void TaskScheduler::stop() {
    running_ = false;
    
    if (scheduler_thread_.joinable()) {
        scheduler_thread_.join();
    }
    
    thread_pool_->shutdown();
}

void TaskScheduler::waitForAll() {
    while (getPendingTaskCount() > 0 || thread_pool_->getQueueSize() > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void TaskScheduler::schedulerLoop() {
    while (running_) {
        std::shared_ptr<Task> task;
        
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (!task_queue_.empty()) {
                task = task_queue_.top();
                task_queue_.pop();
            }
        }
        
        if (task && task->getStatus() == TaskStatus::PENDING) {
            thread_pool_->enqueue([this, task]() {
                executeTask(task);
            });
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TaskScheduler::executeTask(std::shared_ptr<Task> task) {
    std::cout << "Executing: " << task->getName() 
              << " (Priority: " << static_cast<int>(task->getPriority()) << ")" << std::endl;
    
    task->execute();
    completed_tasks_++;
    
    std::cout << "Completed: " << task->getName() << std::endl;
}

size_t TaskScheduler::getPendingTaskCount() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return task_queue_.size();
}

size_t TaskScheduler::getCompletedTaskCount() const {
    return completed_tasks_;
}