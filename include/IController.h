#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "ControllerComponent.h"
#include <memory>

class IController : public ControllerComponent {
private:
    double ki;
    double integralMin;
    double integralMax;
    double integralState;

public:
    IController(double ki, double integralMin, double integralMax);

    double compute(double input, double timeStep) override;
    void reset() override;
    std::unique_ptr<ControllerComponent> clone() override;

    double getIntegralState();
    void setIntegralLimits(double integralMin, double integralMax);
};

#endif
