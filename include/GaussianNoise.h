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

    
    void setMean(double mean);
    void setStandardDeviation(double standardDeviation);
    void setSeed(std::uint32_t seed);

    double getMean() const;
    double getStandardDeviation() const;
    std::uint32_t getSeed() const;
    bool setParameter(const std::string& name, double value) override;
    bool getParameter(const std::string& name, double& value) const override;

private:
    double mean_;
    double standardDeviation_;
    std::uint32_t seed_;
    std::mt19937 generator_;

    std::normal_distribution<double> distribution_;
};
