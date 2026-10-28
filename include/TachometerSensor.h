#pragma once

#include <memory>
#include <string>

#include "Sensor.h"
#include "NoiseGenerator.h"

class TachometerSensor : public Sensor {
public:
    explicit TachometerSensor(std::unique_ptr<NoiseGenerator> noiseGenerator);

    double measure(double trueValue) override;
    void reset() override;
    void setNoiseGenerator(std::unique_ptr<NoiseGenerator> noiseGenerator);
    double getLastMeasurement();
    double getLastNoise();

  
    bool configureNoise(const std::string& name, double value) override;

    // Read a setting back from the injected noise generator.
    bool getNoiseParameter(const std::string& name, double& value) const;

private:
    std::unique_ptr<NoiseGenerator> noiseGenerator_;
    double lastMeasurement_;
    double lastNoise_;
};
