#include "Motor.h"

Motor::Motor(double initialVelocity)
    : initialVelocity(initialVelocity),
      angularVelocity(initialVelocity),
      angularPosition(0.0),
      current(0.0),
      loadTorque(0.0)
{
}

Motor::~Motor()
{
}

double Motor::getAngularVelocity() const
{
    return angularVelocity;
}

double Motor::getAngularPosition() const
{
    return angularPosition;
}

double Motor::getCurrent() const
{
    return current;
}

double Motor::getLoadTorque() const
{
    return loadTorque;
}

void Motor::setLoadTorque(double loadTorque)
{
    this->loadTorque = loadTorque;
}

void Motor::updateState(double angularVelocity,
                        double angularPosition,
                        double current)
{
    this->angularVelocity = angularVelocity;
    this->angularPosition = angularPosition;
    this->current = current;
}
