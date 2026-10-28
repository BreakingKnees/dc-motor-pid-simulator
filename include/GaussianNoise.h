#pragma once

#include <cstdint>
#include <random>
#include <string>

#include "NoiseGenerator.h"

class GaussianNoise : public NoiseGenerator {
public:
    // Throws std::invalid_argument if mean is not finite, or if
    // standardDeviation is not finite or is negative.
    GaussianNoise(double mean, double standardDeviation, std::uint32_t seed);

    double sample() override;
    void reset() override;
    std::string getName() override;

    // ---- Dynamic configuration (added) -------------------------------------
    // Typed setters. Throw std::invalid_argument on invalid input and leave
    // the generator unchanged. The random stream is NOT restarted by
    // setMean/setStandardDeviation, so changing them mid-run just shifts /
    // rescales the noise from the next sample onward. setSeed() DOES restart
    // the stream from the new seed.
    void setMean(double mean);
    void setStandardDeviation(double standardDeviation);
    void setSeed(std::uint32_t seed);

    double getMean() const;
    double getStandardDeviation() const;
    std::uint32_t getSeed() const;

    // Name-based interface from NoiseGenerator: "mean", "stddev", "seed".
    bool setParameter(const std::string& name, double value) override;
    bool getParameter(const std::string& name, double& value) const override;

private:
    double mean_;
    double standardDeviation_;
    std::uint32_t seed_;
    std::mt19937 generator_;
    // Always a standard normal N(0,1); mean/stddev are applied in sample().
    // This keeps stddev == 0 safe and makes runtime changes trivial.
    std::normal_distribution<double> distribution_;
};
