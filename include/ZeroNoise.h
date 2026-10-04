#pragma once

#include "NoiseGenerator.h"

class ZeroNoise : public NoiseGenerator {
public:
    double sample() override;
    void reset() override;
    std::string getName() override;
};
