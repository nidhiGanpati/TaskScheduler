#include "Task.h"
#include <iostream>

Task::Task(TaskId id, TaskFunction func, TaskPriority priority)
    : id_(id), name_("Task_" + std::to_string(id)), function_(func), 
      priority_(priority), status_(TaskStatus::PENDING) {}

Task::Task(TaskId id, TaskFunction func, const std::string& name, TaskPriority priority)
    : id_(id), name_(name), function_(func), 
      priority_(priority), status_(TaskStatus::PENDING) {}

void Task::execute() {
    try {
        status_ = TaskStatus::RUNNING;
        function_();
        status_ = TaskStatus::COMPLETED;
    } catch (const std::exception& e) {
        status_ = TaskStatus::FAILED;
        std::cerr << "Task " << id_ << " failed: " << e.what() << std::endl;
    }
}