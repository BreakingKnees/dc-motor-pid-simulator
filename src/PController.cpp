#include "PController.h"

PController::PController(double kp)
    : Gain(kp, "PController") {}

std::unique_ptr<ControllerComponent> PController::clone() {
    return std::make_unique<PController>(getGain());
}
