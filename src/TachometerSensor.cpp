#include "TachometerSensor.h"

#include <utility>

TachometerSensor::TachometerSensor(
    std::unique_ptr<NoiseGenerator> noiseGenerator)
    : noiseGenerator_(std::move(noiseGenerator)),
      lastMeasurement_(0.0),
      lastNoise_(0.0) {
}

double TachometerSensor::measure(double trueValue) {
    if (noiseGenerator_) {
        lastNoise_ = noiseGenerator_->sample();
    } else {
        lastNoise_ = 0.0;
    }

    lastMeasurement_ = trueValue + lastNoise_;
    return lastMeasurement_;
}

void TachometerSensor::reset() {
    lastMeasurement_ = 0.0;
    lastNoise_ = 0.0;

    if (noiseGenerator_) {
        noiseGenerator_->reset();
    }
}

void TachometerSensor::setNoiseGenerator(
    std::unique_ptr<NoiseGenerator> noiseGenerator) {
    noiseGenerator_ = std::move(noiseGenerator);
}

double TachometerSensor::getLastMeasurement() {
    return lastMeasurement_;
}

double TachometerSensor::getLastNoise() {
    return lastNoise_;
}
