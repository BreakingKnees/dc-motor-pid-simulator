#include "UniformNoise.h"

UniformNoise::UniformNoise(
    double lowerBound,
    double upperBound,
    std::uint32_t seed)
    : lowerBound_(lowerBound),
      upperBound_(upperBound),
      seed_(seed),
      generator_(seed),
      distribution_(lowerBound, upperBound) {
}

double UniformNoise::sample() {
    return distribution_(generator_);
}

void UniformNoise::reset() {
    generator_.seed(seed_);
    distribution_.reset();
}

std::string UniformNoise::getName() {
    return "UniformNoise";
}
