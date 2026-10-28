#include "SimulationConfig.h"
#include <iostream>
#include <cstdlib>

using namespace std;

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
      motorType(MotorType::BRUSHED_DC),
      outputFile("output.csv")
{
}

string SimulationConfig::getUsage()
{
    return "Usage: simulator [options]\n"
           "  --motor <brushed|servo>   Motor model (default brushed)\n";
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

uint32_t SimulationConfig::getSeed() const
{
    return seed;
}

NoiseType SimulationConfig::getNoiseType() const
{
    return noiseType;
}

MotorType SimulationConfig::getMotorType() const
{
    return motorType;
}

string SimulationConfig::getOutputFile() const
{
    return outputFile;
}

SimulationConfig SimulationConfig::fromArguments(int argc, char* argv[])
{
    SimulationConfig config;
    
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        
        if (arg == "--help" || arg == "-h")
        {
            config.helpFlag = true;
        }
        else if (arg == "--duration" && i + 1 < argc)
        {
            config.duration = atof(argv[++i]);
        }
        else if (arg == "--timestep" && i + 1 < argc)
        {
            config.timeStep = atof(argv[++i]);
        }
        else if (arg == "--target" && i + 1 < argc)
        {
            config.targetSpeed = atof(argv[++i]);
        }
        else if (arg == "--output" && i + 1 < argc)
        {
            config.outputFile = argv[++i];
        }
        else if (arg == "--kp" && i + 1 < argc)
        {
            config.kp = atof(argv[++i]);
        }
        else if (arg == "--ki" && i + 1 < argc)
        {
            config.ki = atof(argv[++i]);
        }
        else if (arg == "--kd" && i + 1 < argc)
        {
            config.kd = atof(argv[++i]);
        }
        else if (arg == "--voltage-limit" && i + 1 < argc)
        {
            config.voltageLimit = atof(argv[++i]);
        }
        else if (arg == "--motor" && i + 1 < argc)
        {
            string value = argv[++i];
            if (value == "servo")
            {
                config.motorType = MotorType::SERVO;
            }
            else if (value == "brushed")
            {
                config.motorType = MotorType::BRUSHED_DC;
            }
            else
            {
                cerr << "Unknown motor type '" << value
                     << "' (expected brushed or servo); using brushed.\n";
            }
        }
        else if (arg == "--noise-level" && i + 1 < argc)
        {
            config.noiseLevel = atof(argv[++i]);
            if (config.noiseLevel > 0)
            {
                config.noiseType = NoiseType::GAUSSIAN;
            }
        }
    }
    
    return config;
}
