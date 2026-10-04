#pragma once
#include <string>
#include <cstdint>

enum class NoiseType {
    NONE,
    GAUSSIAN,
    UNIFORM
};

class SimulationConfig {
private:
    bool helpFlag;
    double duration;
    double timeStep;
    double targetSpeed;
    double kp, ki, kd;
    double voltageLimit;
    double integralLimit;
    double noiseLevel;
    double loadTorque;
    double loadStepTime;
    std::uint32_t seed;
    NoiseType noiseType;
    std::string outputFile;

public:
    SimulationConfig();

    static SimulationConfig fromArguments(int argc, char* argv[]);
    static std::string getUsage();

    void validate();
    bool wantsHelp() const;

    double getDuration() const;
    double getTimeStep() const;
    double getTargetSpeed() const;
    double getKp() const;
    double getKi() const;
    double getKd() const;
    double getVoltageLimit() const;
    double getIntegralLimit() const;
    double getNoiseLevel() const;
    double getLoadTorque() const;
    double getLoadStepTime() const;

    std::uint32_t getSeed() const;
    NoiseType getNoiseType() const;
    std::string getOutputFile() const;
};
