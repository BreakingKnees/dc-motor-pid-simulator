#include "SimulationConfig.h"
#include <iostream>
#include <cstdlib>

SimulationConfig::SimulationConfig()
    : helpFlag(false),
      duration(10.0),
      timeStep(0.01),
      targetSpeed(100.0),
      kp(1.0),
      ki(0.0),
      kd(0.0),
      voltageLimit(12.0),
      integralLimit(100.0),
      noiseLevel(0.0),
      loadTorque(0.0),
      loadStepTime(0.0),
      seed(42),
      noiseType(NoiseType::NONE),
      outputFile("output.csv")
{
}

std::string SimulationConfig::getUsage()
{
    return "Usage: simulator [options]\n";
}

void SimulationConfig::validate()
{
    // simple dummy validation
}

bool SimulationConfig::wantsHelp() const
{
    return helpFlag;
}

double SimulationConfig::getDuration() const
{
    return duration;
}

double SimulationConfig::getTimeStep() const
{
    return timeStep;
}

double SimulationConfig::getTargetSpeed() const
{
    return targetSpeed;
}

double SimulationConfig::getKp() const
{
    return kp;
}

double SimulationConfig::getKi() const
{
    return ki;
}

double SimulationConfig::getKd() const
{
    return kd;
}

double SimulationConfig::getVoltageLimit() const
{
    return voltageLimit;
}

double SimulationConfig::getIntegralLimit() const
{
    return integralLimit;
}

double SimulationConfig::getNoiseLevel() const
{
    return noiseLevel;
}

double SimulationConfig::getLoadTorque() const
{
    return loadTorque;
}

double SimulationConfig::getLoadStepTime() const
{
    return loadStepTime;
}

std::uint32_t SimulationConfig::getSeed() const
{
    return seed;
}

NoiseType SimulationConfig::getNoiseType() const
{
    return noiseType;
}

std::string SimulationConfig::getOutputFile() const
{
    return outputFile;
}

SimulationConfig SimulationConfig::fromArguments(int argc, char* argv[])
{
    SimulationConfig config;
    
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h")
        {
            config.helpFlag = true;
        }
        else if (arg == "--duration" && i + 1 < argc)
        {
            config.duration = std::atof(argv[++i]);
        }
        else if (arg == "--timestep" && i + 1 < argc)
        {
            config.timeStep = std::atof(argv[++i]);
        }
        else if (arg == "--target" && i + 1 < argc)
        {
            config.targetSpeed = std::atof(argv[++i]);
        }
        else if (arg == "--output" && i + 1 < argc)
        {
            config.outputFile = argv[++i];
        }
        else if (arg == "--kp" && i + 1 < argc)
        {
            config.kp = std::atof(argv[++i]);
        }
        else if (arg == "--ki" && i + 1 < argc)
        {
            config.ki = std::atof(argv[++i]);
        }
        else if (arg == "--kd" && i + 1 < argc)
        {
            config.kd = std::atof(argv[++i]);
        }
        else if (arg == "--voltage-limit" && i + 1 < argc)
        {
            config.voltageLimit = std::atof(argv[++i]);
        }
        else if (arg == "--noise-level" && i + 1 < argc)
        {
            config.noiseLevel = std::atof(argv[++i]);
            // If noise level is provided, switch to Gaussian noise automatically
            if (config.noiseLevel > 0) {
                config.noiseType = NoiseType::GAUSSIAN;
            }
        }
    }
    
    return config;
}
