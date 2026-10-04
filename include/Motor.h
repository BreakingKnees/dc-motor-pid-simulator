#ifndef MOTOR_H
#define MOTOR_H

#include <string>

class Motor {
public:
    Motor(double initialVelocity);
    virtual ~Motor();

    virtual void step(double appliedVoltage, double timeStep) = 0;
    virtual void reset() = 0;
    virtual std::string getName() const = 0;

    double getAngularVelocity() const;
    double getAngularPosition() const;
    double getCurrent() const;
    double getLoadTorque() const;

    void setLoadTorque(double loadTorque);

protected:
    void updateState(double angularVelocity,
                     double angularPosition,
                     double current);

    double initialVelocity;
    double angularVelocity;
    double angularPosition;
    double current;
    double loadTorque;
};

#endif
