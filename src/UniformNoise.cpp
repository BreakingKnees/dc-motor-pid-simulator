#include "UniformNoise.h"

#include <cmath>
#include <stdexcept>

UniformNoise::UniformNoise(
    double lowerBound,
    double upperBound,
    std::uint32_t seed)
    : lowerBound_(0.0),
      upperBound_(0.0),
      seed_(seed),
      generator_(seed),
      distribution_(0.0, 1.0) {
    setBounds(lowerBound, upperBound);
}

double UniformNoise::sample() {
    // Map a U[0,1) draw onto [lower, upper). If lower == upper this is
    // simply that constant value.
    return lowerBound_ + (upperBound_ - lowerBound_) * distribution_(generator_);
}

void UniformNoise::reset() {
    generator_.seed(seed_);
    distribution_.reset();
}

std::string UniformNoise::getName() {
    return "UniformNoise";
}

void UniformNoise::setBounds(double lowerBound, double upperBound) {
    if (!std::isfinite(lowerBound) || !std::isfinite(upperBound)) {
        throw std::invalid_argument("UniformNoise: bounds must be finite");
    }
    if (lowerBound > upperBound) {
        throw std::invalid_argument(
            "UniformNoise: lower bound must be <= upper bound");
    }
    lowerBound_ = lowerBound;
    upperBound_ = upperBound;
}

void UniformNoise::setLowerBound(double lowerBound) {
    setBounds(lowerBound, upperBound_);
}

void UniformNoise::setUpperBound(double upperBound) {
    setBounds(lowerBound_, upperBound);
}

void UniformNoise::setSeed(std::uint32_t seed) {
    seed_ = seed;
    generator_.seed(seed_);
    distribution_.reset();
}

double UniformNoise::getLowerBound() const {
    return lowerBound_;
}

double UniformNoise::getUpperBound() const {
    return upperBound_;
}

std::uint32_t UniformNoise::getSeed() const {
    return seed_;
}

bool UniformNoise::setParameter(const std::string& name, double value) {
    try {
        if (name == "lower") {
            setLowerBound(value);
            return true;
        }
        if (name == "upper") {
            setUpperBound(value);
            return true;
        }
        if (name == "seed") {
            if (!std::isfinite(value) || value < 0.0 ||
                value > 4294967295.0 || value != std::floor(value)) {
                return false;
            }
            setSeed(static_cast<std::uint32_t>(value));
            return true;
        }
    } catch (const std::invalid_argument&) {
        return false;  // invalid value: nothing was changed
    }
    return false;  // unknown name
}

bool UniformNoise::getParameter(const std::string& name, double& value) const {
    if (name == "lower") {
        value = lowerBound_;
        return true;
    }
    if (name == "upper") {
        value = upperBound_;
        return true;
    }
    if (name == "seed") {
        value = static_cast<double>(seed_);
        return true;
    }
    return false;
}
