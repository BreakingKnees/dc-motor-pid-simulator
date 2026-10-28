#pragma once

#include <string>

class NoiseGenerator {
public:
    virtual ~NoiseGenerator() = default;
    virtual double sample() = 0;
    virtual void reset() = 0;
    virtual std::string getName() = 0;

    // ---- Dynamic configuration (added) -------------------------------------
    // Change a noise setting by name while the program is running, instead of
    // having it fixed at construction. Recognised names depend on the
    // concrete generator:
    //   GaussianNoise : "mean", "stddev", "seed"
    //   UniformNoise  : "lower", "upper", "seed"
    //   ZeroNoise     : (none)
    //
    // Returns true if the setting was applied. Returns false (and changes
    // nothing) if the name is unknown for this generator or the value is
    // invalid (NaN/inf, negative stddev, lower > upper, ...).
    //
    // These are NOT pure virtual, so existing subclasses keep compiling and
    // simply report "not configurable".
    virtual bool setParameter(const std::string& name, double value) {
        (void)name;
        (void)value;
        return false;
    }

    // Read a setting by name. Returns true and writes `value` if the name is
    // known, otherwise returns false and leaves `value` untouched.
    virtual bool getParameter(const std::string& name, double& value) const {
        (void)name;
        (void)value;
        return false;
    }
};
