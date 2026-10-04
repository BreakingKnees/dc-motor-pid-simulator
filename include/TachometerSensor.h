#pragma once

#include <memory>

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

private:
    std::unique_ptr<NoiseGenerator> noiseGenerator_;
    double lastMeasurement_;
    double lastNoise_;
};
