#include "ControllerComponent.h"

ControllerComponent::ControllerComponent(std::string name) : name(name) {}

ControllerComponent::~ControllerComponent() {}

std::string ControllerComponent::getName() {
    return name;
}
