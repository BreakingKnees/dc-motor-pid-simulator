#ifndef COMPOSITECONTROLLER_H
#define COMPOSITECONTROLLER_H

#include "ControllerComponent.h"
#include <cstddef>
#include <memory>
#include <vector>

class CompositeController : public ControllerComponent {
private:
    double outputMin;
    double outputMax;
    std::vector<std::unique_ptr<ControllerComponent>> components;

public:
    CompositeController(double outputMin, double outputMax);

    void addComponent(std::unique_ptr<ControllerComponent> component);
    std::size_t getComponentCount();

    double compute(double input, double timeStep) override;
    void reset() override;
    std::unique_ptr<ControllerComponent> clone() override;

    void setOutputLimits(double outputMin, double outputMax);
};

#endif
