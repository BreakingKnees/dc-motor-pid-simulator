#include "IController.h"

IController::IController(double ki, double integralMin, double integralMax)
    : ControllerComponent("IController"),
      ki(ki),
      integralMin(integralMin),
      integralMax(integralMax),
      integralState(0.0) {}

double IController::compute(double input, double timeStep) {
    integralState += input * timeStep;

    if (integralState > integralMax) {
        integralState = integralMax;
    } else if (integralState < integralMin) {
        integralState = integralMin;
    }

    return ki * integralState;
}

void IController::reset() {
    integralState = 0.0;
}

std::unique_ptr<ControllerComponent> IController::clone() {
    return std::make_unique<IController>(ki, integralMin, integralMax);
}

double IController::getIntegralState() {
    return integralState;
}

void IController::setIntegralLimits(double integralMin, double integralMax) {
    this->integralMin = integralMin;
    this->integralMax = integralMax;

    if (integralState > this->integralMax) {
        integralState = this->integralMax;
    } else if (integralState < this->integralMin) {
        integralState = this->integralMin;
    }
}
