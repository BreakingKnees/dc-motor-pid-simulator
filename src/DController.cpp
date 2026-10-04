#include "DController.h"

DController::DController(double kd, double filterAlpha)
    : ControllerComponent("DController"),
      kd(kd),
      filterAlpha(filterAlpha),
      previousInput(0.0),
      previousDerivative(0.0),
      isFirstRun(true) {}

double DController::compute(double input, double timeStep) {
    double currentDerivative = 0.0;

    if (isFirstRun) {
        currentDerivative = 0.0;
        isFirstRun = false;
    } else {
        double rawDerivative = (input - previousInput) / timeStep;
        currentDerivative = (1.0 - filterAlpha) * rawDerivative
                          + filterAlpha * previousDerivative;
    }

    previousInput = input;
    previousDerivative = currentDerivative;

    return kd * currentDerivative;
}

void DController::reset() {
    previousInput = 0.0;
    previousDerivative = 0.0;
    isFirstRun = true;
}

std::unique_ptr<ControllerComponent> DController::clone() {
    return std::make_unique<DController>(kd, filterAlpha);
}
