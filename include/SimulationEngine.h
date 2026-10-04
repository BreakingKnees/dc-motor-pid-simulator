#pragma once
#include <memory>
#include <cstddef>
#include "Motor.h"
#include "ControllerComponent.h"
#include "Sensor.h"
#include "ThreadSafeQueue.h"
#include "LogRecord.h"
#include "SimulationConfig.h"

class SimulationEngine {
private:
    std::unique_ptr<Motor> motor_;
    std::unique_ptr<ControllerComponent> controller_;
    std::unique_ptr<Sensor> sensor_;
    ThreadSafeQueue<LogRecord>& queue_;
    SimulationConfig config_;
    
    double currentTime_;
    std::size_t stepCount_;

public:
    SimulationEngine(std::unique_ptr<Motor> motor,
                     std::unique_ptr<ControllerComponent> controller,
                     std::unique_ptr<Sensor> sensor,
                     ThreadSafeQueue<LogRecord>& queue,
                     const SimulationConfig& config);

    void run();
    void step();
    double getCurrentTime() const;
    std::size_t getStepCount() const;
};
