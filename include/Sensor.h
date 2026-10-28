#pragma once

#include <string>

class Sensor {
public:
    virtual ~Sensor() = default;

    virtual double measure(double trueValue) = 0;
    virtual void reset() = 0;

    // ---- Dynamic noise configuration (added) -------------------------------
    // Forward a noise setting ("mean", "stddev", "lower", "upper", "seed")
    // to whatever noise source this sensor uses. Returns true if applied.
    // Default: a sensor with no configurable noise returns false.
    virtual bool configureNoise(const std::string& name, double value) {
        (void)name;
        (void)value;
        return false;
    }
};
