#include "ComponentFactory.h"
#include "TachometerSensor.h"
#include "CSVOutput.h"
#include "ZeroNoise.h"
#include "GaussianNoise.h"
#include "UniformNoise.h"
#include "CompositeController.h"
#include "PController.h"
#include "IController.h"
#include "DController.h"

#include "BrushedDCMotor.h"

using namespace std;

unique_ptr<Motor> ComponentFactory::createMotor(const SimulationConfig& config)
{
    (void)config;
    // Hardcoding reasonable physical constants for a small 12V DC motor:
    // resistance, inductance, torqueConstant, backEmfConstant, inertia, viscousFriction, initialVelocity
    return make_unique<BrushedDCMotor>(2.0, 0.5, 0.1, 0.1, 0.01, 0.001, 0.0);
}

unique_ptr<ControllerComponent> ComponentFactory::createController(const SimulationConfig& config)
{
    auto controller = make_unique<CompositeController>(
        -config.getVoltageLimit(), config.getVoltageLimit()
    );
    
    controller->addComponent(make_unique<PController>(config.getKp()));
    controller->addComponent(make_unique<IController>(
        config.getKi(), -config.getIntegralLimit(), config.getIntegralLimit()
    ));
    controller->addComponent(make_unique<DController>(config.getKd(), 1.0)); // 1.0 is no filter
    
    return controller;
}

unique_ptr<NoiseGenerator> ComponentFactory::createNoise(const SimulationConfig& config)
{
    switch (config.getNoiseType())
    {
        case NoiseType::GAUSSIAN:
            return make_unique<GaussianNoise>(0.0, config.getNoiseLevel(), config.getSeed());
        case NoiseType::UNIFORM:
            return make_unique<UniformNoise>(-config.getNoiseLevel(), config.getNoiseLevel(), config.getSeed());
        case NoiseType::NONE:
        default:
            return make_unique<ZeroNoise>();
    }
}

unique_ptr<Sensor> ComponentFactory::createSensor(const SimulationConfig& config)
{
    return make_unique<TachometerSensor>(createNoise(config));
}

unique_ptr<OutputLogger> ComponentFactory::createLogger(const SimulationConfig& config)
{
    // 6 decimal places of precision is typical for simulation CSV output
    return make_unique<CSVOutput>(config.getOutputFile(), 6);
}
