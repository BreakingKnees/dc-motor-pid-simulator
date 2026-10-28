#pragma once

#include <cstdint>
#include <random>
#include <string>

#include "NoiseGenerator.h"

class UniformNoise : public NoiseGenerator {
public:
    // Throws std::invalid_argument if a bound is not finite or
    // lowerBound > upperBound.
    UniformNoise(double lowerBound, double upperBound, std::uint32_t seed);

    double sample() override;
    void reset() override;
    std::string getName() override;

    // ---- Dynamic configuration (added) -------------------------------------
    // Typed setters. Throw std::invalid_argument on invalid input and leave
    // the generator unchanged. Changing bounds does not restart the random
    // stream; setSeed() does.
    //
    // To move the whole range (e.g. [-1,1] -> [5,10]) use setBounds(): setting
    // the bounds one at a time can fail part-way because lower would
    // temporarily exceed upper.
    void setBounds(double lowerBound, double upperBound);
    void setLowerBound(double lowerBound);
    void setUpperBound(double upperBound);
    void setSeed(std::uint32_t seed);

    double getLowerBound() const;
    double getUpperBound() const;
    std::uint32_t getSeed() const;

    // Name-based interface from NoiseGenerator: "lower", "upper", "seed".
    bool setParameter(const std::string& name, double value) override;
    bool getParameter(const std::string& name, double& value) const override;

private:
    double lowerBound_;
    double upperBound_;
    std::uint32_t seed_;
    std::mt19937 generator_;
    // Always U[0,1); bounds are applied in sample(). Keeps lower == upper
    // safe and makes runtime changes trivial.
    std::uniform_real_distribution<double> distribution_;
};
