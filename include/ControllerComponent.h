#ifndef CONTROLLERCOMPONENT_H
#define CONTROLLERCOMPONENT_H

#include <memory>
#include <string>

class ControllerComponent {
private:
    std::string name;

public:
    ControllerComponent(std::string name);
    virtual ~ControllerComponent();

    virtual double compute(double input, double timeStep) = 0;
    virtual void reset() = 0;
    virtual std::unique_ptr<ControllerComponent> clone() = 0;

    std::string getName();
};

#endif
