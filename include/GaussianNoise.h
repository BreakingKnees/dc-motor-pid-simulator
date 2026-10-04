#pragma once

#include <cstdint>
#include <random>
#include <string>

#include "NoiseGenerator.h"

class GaussianNoise : public NoiseGenerator {
public:
    GaussianNoise(double mean, double standardDeviation, std::uint32_t seed);

    double sample() override;
    void reset() override;
    std::string getName() override;

private:
    double mean_;
    double standardDeviation_;
    std::uint32_t seed_;
    std::mt19937 generator_;
    std::normal_distribution<double> distribution_;
};
