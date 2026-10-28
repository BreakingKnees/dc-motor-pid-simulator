#include "GaussianNoise.h"

#include <cmath>
#include <stdexcept>

GaussianNoise::GaussianNoise(
    double mean,
    double standardDeviation,
    std::uint32_t seed)
    : mean_(0.0),
      standardDeviation_(0.0),
      seed_(seed),
      generator_(seed),
      distribution_(0.0, 1.0) {
    setMean(mean);
    setStandardDeviation(standardDeviation);
}

double GaussianNoise::sample() {
    // A standard-normal draw scaled and shifted. With stddev == 0 this is
    // just the mean (a constant offset).
    return mean_ + standardDeviation_ * distribution_(generator_);
}

void GaussianNoise::reset() {
    generator_.seed(seed_);
    distribution_.reset();
}

std::string GaussianNoise::getName() {
    return "GaussianNoise";
}

void GaussianNoise::setMean(double mean) {
    if (!std::isfinite(mean)) {
        throw std::invalid_argument("GaussianNoise: mean must be finite");
    }
    mean_ = mean;
}

void GaussianNoise::setStandardDeviation(double standardDeviation) {
    if (!std::isfinite(standardDeviation) || standardDeviation < 0.0) {
        throw std::invalid_argument(
            "GaussianNoise: standard deviation must be finite and >= 0");
    }
    standardDeviation_ = standardDeviation;
}

void GaussianNoise::setSeed(std::uint32_t seed) {
    seed_ = seed;
    generator_.seed(seed_);
    distribution_.reset();
}

double GaussianNoise::getMean() const {
    return mean_;
}

double GaussianNoise::getStandardDeviation() const {
    return standardDeviation_;
}

std::uint32_t GaussianNoise::getSeed() const {
    return seed_;
}

bool GaussianNoise::setParameter(const std::string& name, double value) {
    try {
        if (name == "mean") {
            setMean(value);
            return true;
        }
        if (name == "stddev") {
            setStandardDeviation(value);
            return true;
        }
        if (name == "seed") {
            // Seed must be a whole number that fits in 32 bits.
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

bool GaussianNoise::getParameter(const std::string& name, double& value) const {
    if (name == "mean") {
        value = mean_;
        return true;
    }
    if (name == "stddev") {
        value = standardDeviation_;
        return true;
    }
    if (name == "seed") {
        value = static_cast<double>(seed_);
        return true;
    }
    return false;
}
