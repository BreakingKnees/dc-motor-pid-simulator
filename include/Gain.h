#ifndef GAIN_H
#define GAIN_H

#include "ControllerComponent.h"
#include <memory>
#include <string>

class Gain : public ControllerComponent {
private:
    double gain;

public:
    Gain(double gain, std::string name);

    double compute(double input, double timeStep) override;
    void reset() override;
    std::unique_ptr<ControllerComponent> clone() override;

    double getGain();
    void setGain(double gain);
};

#endif
