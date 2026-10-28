#pragma once

#include <string>

class NoiseGenerator {
public:
    virtual ~NoiseGenerator() = default;
    virtual double sample() = 0;
    virtual void reset() = 0;
    virtual std::string getName() = 0;


    virtual bool setParameter(const std::string& name, double value) {
        (void)name;
        (void)value;
        return false;
    }

    virtual bool getParameter(const std::string& name, double& value) const {
        (void)name;
        (void)value;
        return false;
    }
};
