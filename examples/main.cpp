#include "TaskScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>

void exampleTask(const std::string& message, int duration_ms) {
    std::cout << message << " - Starting" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    std::cout << message << " - Done!" << std::endl;
}

int main() {
    std::cout << "=== Task Scheduler Demo ===" << std::endl;
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads" << std::endl;
    
    TaskScheduler scheduler(4);
    
    // Add tasks with different priorities
    scheduler.addTask(
        []() { exampleTask("Low Priority Task", 1000); },
        TaskPriority::LOW,
        "LowTask1"
    );
    
    scheduler.addTask(
        []() { exampleTask("Normal Priority Task", 800); },
        TaskPriority::NORMAL,
        "NormalTask1"
    );
    
    scheduler.addTask(
        []() { exampleTask("High Priority Task", 600); },
        TaskPriority::HIGH,
        "HighTask1"
    );
    
    scheduler.addTask(
        []() { exampleTask("Critical Priority Task", 400); },
        TaskPriority::CRITICAL,
        "CriticalTask1"
    );
    
    // Add more tasks
    for (int i = 0; i < 5; ++i) {
        scheduler.addTask(
            [i]() { exampleTask("Task " + std::to_string(i), 500); },
            TaskPriority::NORMAL
        );
    }
    
    std::cout << "\nStarting scheduler..." << std::endl;
    scheduler.start();
    
    // Wait for all tasks to complete
    scheduler.waitForAll();
    
    std::cout << "\nAll tasks completed!" << std::endl;
    std::cout << "Total completed: " << scheduler.getCompletedTaskCount() << std::endl;
    
    scheduler.stop();
    
    return 0;
}