#pragma once
#include <memory>
#include "SimulationConfig.h"
#include "Motor.h"
#include "ControllerComponent.h"
#include "Sensor.h"
#include "NoiseGenerator.h"
#include "OutputLogger.h"

class ComponentFactory {
public:
    static std::unique_ptr<Motor> createMotor(const SimulationConfig& config);
    static std::unique_ptr<ControllerComponent> createController(const SimulationConfig& config);
    static std::unique_ptr<Sensor> createSensor(const SimulationConfig& config);
    static std::unique_ptr<NoiseGenerator> createNoise(const SimulationConfig& config);
    static std::unique_ptr<OutputLogger> createLogger(const SimulationConfig& config);
};
