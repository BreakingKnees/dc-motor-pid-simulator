#ifndef BRUSHED_DC_MOTOR_H
#define BRUSHED_DC_MOTOR_H

#include "Motor.h"
#include <string>

class BrushedDCMotor : public Motor {
public:
    BrushedDCMotor(
        double resistance,
        double inductance,
        double torqueConstant,
        double backEmfConstant,
        double inertia,
        double viscousFriction,
        double initialVelocity
    );

    void step(double appliedVoltage, double timeStep) override;
    void reset() override;
    std::string getName() const override;

    double getBackEmf() const;
    double getElectromagneticTorque() const;

    double getResistance() const;
    double getInductance() const;
    double getTorqueConstant() const;
    double getBackEmfConstant() const;
    double getInertia() const;
    double getViscousFriction() const;

private:
    double resistance;
    double inductance;
    double torqueConstant;
    double backEmfConstant;
    double inertia;
    double viscousFriction;

    double backEmf;
    double electromagneticTorque;
};

#endif
