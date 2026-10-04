#include "GaussianNoise.h"

GaussianNoise::GaussianNoise(
    double mean,
    double standardDeviation,
    std::uint32_t seed)
    : mean_(mean),
      standardDeviation_(standardDeviation),
      seed_(seed),
      generator_(seed),
      distribution_(mean, standardDeviation) {
}

double GaussianNoise::sample() {
    return distribution_(generator_);
}

void GaussianNoise::reset() {
    generator_.seed(seed_);
    distribution_.reset();
}

std::string GaussianNoise::getName() {
    return "GaussianNoise";
}
