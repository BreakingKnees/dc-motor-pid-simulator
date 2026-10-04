#ifndef DCONTROLLER_H
#define DCONTROLLER_H

#include "ControllerComponent.h"
#include <memory>

class DController : public ControllerComponent {
private:
    double kd;
    double filterAlpha;
    double previousInput;
    double previousDerivative;
    bool isFirstRun;

public:
    DController(double kd, double filterAlpha);

    double compute(double input, double timeStep) override;
    void reset() override;
    std::unique_ptr<ControllerComponent> clone() override;
};

#endif
