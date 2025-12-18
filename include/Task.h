#ifndef TASK_H
#define TASK_H

#include <functional>
#include <string>
#include <chrono>

enum class TaskPriority {
    LOW = 0,
    NORMAL = 1,
    HIGH = 2,
    CRITICAL = 3
};

enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED
};

class Task {
public:
    using TaskFunction = std::function<void()>;
    using TaskId = size_t;

    Task(TaskId id, TaskFunction func, TaskPriority priority = TaskPriority::NORMAL);
    Task(TaskId id, TaskFunction func, const std::string& name, TaskPriority priority = TaskPriority::NORMAL);

    void execute();
    
    TaskId getId() const { return id_; }
    const std::string& getName() const { return name_; }
    TaskPriority getPriority() const { return priority_; }
    TaskStatus getStatus() const { return status_; }
    
    void setStatus(TaskStatus status) { status_ = status; }
    
    bool operator<(const Task& other) const {
        return priority_ < other.priority_;
    }

private:
    TaskId id_;
    std::string name_;
    TaskFunction function_;
    TaskPriority priority_;
    TaskStatus status_;
};

#endif