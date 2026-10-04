#include "ZeroNoise.h"

double ZeroNoise::sample() {
    return 0.0;
}

void ZeroNoise::reset() {
}

std::string ZeroNoise::getName() {
    return "ZeroNoise";
}
