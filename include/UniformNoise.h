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

    void setBounds(double lowerBound, double upperBound);
    void setLowerBound(double lowerBound);
    void setUpperBound(double upperBound);
    void setSeed(std::uint32_t seed);

    double getLowerBound() const;
    double getUpperBound() const;
    std::uint32_t getSeed() const;

    bool setParameter(const std::string& name, double value) override;
    bool getParameter(const std::string& name, double& value) const override;

private:
    double lowerBound_;
    double upperBound_;
    std::uint32_t seed_;
    std::mt19937 generator_;
    std::uniform_real_distribution<double> distribution_;
};
