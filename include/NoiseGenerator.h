#pragma once

#include <string>

class NoiseGenerator {
public:
    virtual ~NoiseGenerator() = default;
    virtual double sample() = 0;
    virtual void reset() = 0;
    virtual std::string getName() = 0;
};
