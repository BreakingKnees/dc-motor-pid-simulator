#include "Gain.h"
#include <utility>

Gain::Gain(double gain, std::string name)
    : ControllerComponent(std::move(name)), gain(gain) {}

double Gain::compute(double input, double timeStep) {
    (void)timeStep;
    return input * gain;
}

void Gain::reset() {
}

std::unique_ptr<ControllerComponent> Gain::clone() {
    return std::make_unique<Gain>(gain, getName());
}

double Gain::getGain() {
    return gain;
}

void Gain::setGain(double gain) {
    this->gain = gain;
}
