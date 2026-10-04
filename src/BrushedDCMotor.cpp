#include "BrushedDCMotor.h"

BrushedDCMotor::BrushedDCMotor(
    double resistance,
    double inductance,
    double torqueConstant,
    double backEmfConstant,
    double inertia,
    double viscousFriction,
    double initialVelocity
)
    : Motor(initialVelocity),
      resistance(resistance),
      inductance(inductance),
      torqueConstant(torqueConstant),
      backEmfConstant(backEmfConstant),
      inertia(inertia),
      viscousFriction(viscousFriction),
      backEmf(0.0),
      electromagneticTorque(0.0)
{
}

void BrushedDCMotor::step(double appliedVoltage, double timeStep)
{
    // Standard brushed DC motor equations:
    //
    // V = R*i + L*(di/dt) + Ke*w
    // J*(dw/dt) = Kt*i - b*w - Tload
    // d(theta)/dt = w
    //
    // Euler integration:
    // new value = old value + derivative * timeStep

    backEmf = backEmfConstant * angularVelocity;

    double currentDerivative = 0.0;

    if (inductance != 0.0)
    {
        currentDerivative =
            (appliedVoltage
             - resistance * current
             - backEmf) / inductance;
    }

    double angularAcceleration =
        (torqueConstant * current
         - viscousFriction * angularVelocity
         - loadTorque) / inertia;

    double newCurrent =
        current + currentDerivative * timeStep;

    double newAngularVelocity =
        angularVelocity + angularAcceleration * timeStep;

    double newAngularPosition =
        angularPosition + angularVelocity * timeStep;

    electromagneticTorque = torqueConstant * newCurrent;

    updateState(
        newAngularVelocity,
        newAngularPosition,
        newCurrent
    );

    backEmf = backEmfConstant * angularVelocity;
}

void BrushedDCMotor::reset()
{
    angularVelocity = initialVelocity;
    angularPosition = 0.0;
    current = 0.0;

    backEmf = 0.0;
    electromagneticTorque = 0.0;
    loadTorque = 0.0;
}

std::string BrushedDCMotor::getName() const
{
    return "BrushedDCMotor";
}

double BrushedDCMotor::getBackEmf() const
{
    return backEmf;
}

double BrushedDCMotor::getElectromagneticTorque() const
{
    return electromagneticTorque;
}

double BrushedDCMotor::getResistance() const
{
    return resistance;
}

double BrushedDCMotor::getInductance() const
{
    return inductance;
}

double BrushedDCMotor::getTorqueConstant() const
{
    return torqueConstant;
}

double BrushedDCMotor::getBackEmfConstant() const
{
    return backEmfConstant;
}

double BrushedDCMotor::getInertia() const
{
    return inertia;
}

double BrushedDCMotor::getViscousFriction() const
{
    return viscousFriction;
}
