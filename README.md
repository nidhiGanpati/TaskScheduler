# Multi-Threaded Task Scheduler

A high-performance task scheduler with priority queue implementation in C++17.

## Features
- ✅ Priority-based task scheduling (LOW, NORMAL, HIGH, CRITICAL)
- ✅ Thread pool with configurable worker threads
- ✅ Thread-safe operations using mutexes and condition variables
- ✅ Asynchronous task execution
- ✅ Task status tracking

## Building

### Windows (Visual Studio)
```bash
cd build
cmake ..
cmake --build . --config Release
```

## Running
```bash
cd build/Release
TaskSchedulerDemo.exe
```

## Usage Example
```cpp
TaskScheduler scheduler(4); // 4 worker threads

scheduler.addTask(
    []() { std::cout << "Hello!" << std::endl; },
    TaskPriority::HIGH,
    "MyTask"
);

scheduler.start();
scheduler.waitForAll();
scheduler.stop();
```

## Architecture
- **Task**: Represents a unit of work with priority
- **ThreadPool**: Manages worker threads
- **TaskScheduler**: Coordinates task execution

## License
MIT License