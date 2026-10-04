#pragma once

#include <cstdint>
#include <random>
#include <string>

#include "NoiseGenerator.h"

class UniformNoise : public NoiseGenerator {
public:
    UniformNoise(double lowerBound, double upperBound, std::uint32_t seed);

    double sample() override;
    void reset() override;
    std::string getName() override;

private:
    double lowerBound_;
    double upperBound_;
    std::uint32_t seed_;
    std::mt19937 generator_;
    std::uniform_real_distribution<double> distribution_;
};
