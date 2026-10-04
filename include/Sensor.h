#pragma once

class Sensor {
public:
    virtual ~Sensor() = default;

    virtual double measure(double trueValue) = 0;
    virtual void reset() = 0;
};
