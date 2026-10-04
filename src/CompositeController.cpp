#include "CompositeController.h"
#include <utility>

CompositeController::CompositeController(double outputMin, double outputMax)
    : ControllerComponent("CompositeController"),
      outputMin(outputMin),
      outputMax(outputMax) {}

void CompositeController::addComponent(std::unique_ptr<ControllerComponent> component) {
    components.push_back(std::move(component));
}

std::size_t CompositeController::getComponentCount() {
    return components.size();
}

double CompositeController::compute(double input, double timeStep) {
    double totalOutput = 0.0;

    for (const auto& component : components) {
        totalOutput += component->compute(input, timeStep);
    }

    if (totalOutput > outputMax) {
        totalOutput = outputMax;
    } else if (totalOutput < outputMin) {
        totalOutput = outputMin;
    }

    return totalOutput;
}

void CompositeController::reset() {
    for (const auto& component : components) {
        component->reset();
    }
}

std::unique_ptr<ControllerComponent> CompositeController::clone() {
    auto clonedController =
        std::make_unique<CompositeController>(outputMin, outputMax);

    for (const auto& component : components) {
        clonedController->addComponent(component->clone());
    }

    return clonedController;
}

void CompositeController::setOutputLimits(double outputMin, double outputMax) {
    this->outputMin = outputMin;
    this->outputMax = outputMax;
}
