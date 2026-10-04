# Multithreaded DC Motor PID Control Simulator

## Overview
A high-performance, object-oriented C++ simulation of a closed-loop DC Motor system. Designed for engineering evaluations, this system simulates the physical dynamics of a brushed DC motor under load and actively stabilizes it using a finely-tuned Proportional-Integral-Derivative (PID) controller. 

The architecture is strictly decoupled and utilizes modern C++14 multithreading to separate the intense physics calculations from the disk I/O logging operations.

## Directory Structure
```text
cpp project/
├── Makefile               # Build automation
├── README.md              # Project documentation
├── build/                 # Compiled object files and simulator executable
├── include/               # Class definitions and interfaces
│   ├── BrushedDCMotor.h   # Plant models
│   ├── CompositeController.h # PID logic
│   ├── ThreadSafeQueue.h  # Concurrency primitives
│   └── ... (23 headers)
└── src/                   # Implementation files
    ├── BrushedDCMotor.cpp
    ├── ComponentFactory.cpp
    ├── main.cpp
    └── ... (19 sources)
```

## Build & Run Instructions

**Prerequisites:** A standard C++14 compliant compiler (e.g., `g++`) and `make`.

To completely rebuild the project and execute the simulation immediately with default parameters:
```bash
make clean
make run
```

To compile the project and run it with custom simulation parameters via the CLI:
```bash
make
./build/simulator --duration 10.0 --target 100.0 --timestep 0.01 --output custom_log.csv
```

## Architecture

### Object-Oriented Design
The codebase relies heavily on SOLID principles and interface-based design to allow independent parallel development across team members:
- **Factory Pattern**: The `ComponentFactory` handles the instantiation of all concrete dependencies (Motor, Controller, Sensor, Logger), injecting them cleanly into the simulation engine.
- **Composite Pattern**: The control logic (`CompositeController`) acts as a unified `ControllerComponent` interface that transparently delegates calculations to its leaf children (`PController`, `IController`, `DController`).

### Multithreading Design
To prevent file I/O latency from blocking the critical physics step calculations, the simulation is heavily parallelized using a classic Producer-Consumer pattern:
- **Producer (`SimulationEngine`)**: Runs on the main thread. Responsible purely for stepping the motor physics, polling the sensor, calculating the PID error, and generating a `LogRecord`.
- **Consumer (`LogConsumer`)**: Runs on a dedicated background thread. Safely pops records from the queue and handles formatting and writing them to the CSV file.
- **`ThreadSafeQueue`**: A custom synchronization primitive using `std::mutex` and `std::condition_variable` that transports `LogRecord` objects across the thread boundary without race conditions, enforcing capacity limits to prevent memory ballooning.
