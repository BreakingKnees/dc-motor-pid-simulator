#include "ServoMotor.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {

// Width (rad/s at the rotor) of the smooth Coulomb-friction transition.
// tanh() avoids the sign() discontinuity that makes Euler integration chatter
// around zero speed.
const double kCoulombSmoothing = 1.0;

// Safety cap so a tiny time constant cannot freeze the simulation.
const long kMaxSubSteps = 100000;

void requirePositive(double value, const char* name)
{
    if (!(value > 0.0))
    {
        throw std::invalid_argument(std::string("ServoMotor: ") + name +
                                    " must be > 0");
    }
}

} // namespace

ServoMotor::ServoMotor(
    double resistance,
    double inductance,
    double torqueConstant,
    double backEmfConstant,
    double rotorInertia,
    double viscousFriction,
    double coulombFriction,
    double gearRatio,
    double gearEfficiency,
    double maxCurrent,
    double initialVelocity
)
    : Motor(initialVelocity),
      resistance(resistance),
      inductance(inductance),
      torqueConstant(torqueConstant),
      backEmfConstant(backEmfConstant),
      rotorInertia(rotorInertia),
      viscousFriction(viscousFriction),
      coulombFriction(coulombFriction),
      gearRatio(gearRatio),
      gearEfficiency(gearEfficiency),
      maxCurrent(maxCurrent),
      maxSubStep(0.0),
      backEmf(0.0),
      electromagneticTorque(0.0),
      currentLimited(false)
{
    requirePositive(resistance, "resistance");
    requirePositive(inductance, "inductance");
    requirePositive(torqueConstant, "torqueConstant");
    requirePositive(backEmfConstant, "backEmfConstant");
    requirePositive(rotorInertia, "rotorInertia");
    requirePositive(gearRatio, "gearRatio");
    requirePositive(maxCurrent, "maxCurrent");

    if (viscousFriction < 0.0 || coulombFriction < 0.0)
    {
        throw std::invalid_argument("ServoMotor: friction must be >= 0");
    }
    if (!(gearEfficiency > 0.0 && gearEfficiency <= 1.0))
    {
        throw std::invalid_argument("ServoMotor: gearEfficiency must be in (0, 1]");
    }

    // Servo windings are fast (small L). The simulation's fixed 10 ms step
    // would make plain Euler unstable, so step() subdivides the requested
    // time step. Keep sub-steps well below the fastest time constant.
    const double electricalTau = inductance / resistance;
    const double mechanicalTau =
        rotorInertia * resistance /
        (torqueConstant * backEmfConstant + viscousFriction * resistance);
    maxSubStep = 0.2 * std::min(electricalTau, mechanicalTau);
}

void ServoMotor::step(double appliedVoltage, double timeStep)
{
    // Rotor-side equations (omega_r = N * omega_out):
    //
    //   V = R*i + L*(di/dt) + Ke*omega_r
    //   J*(domega_r/dt) = Kt*i - b*omega_r - Tc*tanh(omega_r/eps)
    //                     - Tload / (N * eta)
    //   omega_out = omega_r / N,   d(theta_out)/dt = omega_out
    //
    // |i| is clamped to maxCurrent (drive current limit).
    // Explicit Euler, applied over internal sub-steps for stability.

    if (!(timeStep > 0.0))
    {
        return;
    }

    long subSteps = static_cast<long>(std::ceil(timeStep / maxSubStep));
    subSteps = std::max(1L, std::min(subSteps, kMaxSubSteps));
    const double h = timeStep / static_cast<double>(subSteps);

    double omegaOut = angularVelocity;
    double thetaOut = angularPosition;
    double i = current;
    bool limited = false;

    const double reflectedLoadFactor = 1.0 / (gearRatio * gearEfficiency);

    for (long k = 0; k < subSteps; ++k)
    {
        const double omegaRotor = gearRatio * omegaOut;
        const double emf = backEmfConstant * omegaRotor;

        const double di = (appliedVoltage - resistance * i - emf) / inductance;

        const double friction =
            viscousFriction * omegaRotor +
            coulombFriction * std::tanh(omegaRotor / kCoulombSmoothing);

        const double domegaRotor =
            (torqueConstant * i - friction - loadTorque * reflectedLoadFactor)
            / rotorInertia;

        double newI = i + di * h;
        if (newI > maxCurrent)
        {
            newI = maxCurrent;
            limited = true;
        }
        else if (newI < -maxCurrent)
        {
            newI = -maxCurrent;
            limited = true;
        }

        const double newOmegaRotor = omegaRotor + domegaRotor * h;

        thetaOut += omegaOut * h;
        omegaOut = newOmegaRotor / gearRatio;
        i = newI;
    }

    currentLimited = limited;
    electromagneticTorque = torqueConstant * i;
    backEmf = backEmfConstant * gearRatio * omegaOut;

    updateState(omegaOut, thetaOut, i);
}

void ServoMotor::reset()
{
    angularVelocity = initialVelocity;
    angularPosition = 0.0;
    current = 0.0;
    loadTorque = 0.0;

    backEmf = 0.0;
    electromagneticTorque = 0.0;
    currentLimited = false;
}

std::string ServoMotor::getName() const
{
    return "ServoMotor";
}

double ServoMotor::getBackEmf() const
{
    return backEmf;
}

double ServoMotor::getElectromagneticTorque() const
{
    return electromagneticTorque;
}

double ServoMotor::getOutputTorque() const
{
    return gearRatio * gearEfficiency * electromagneticTorque;
}

double ServoMotor::getRotorVelocity() const
{
    return gearRatio * angularVelocity;
}

bool ServoMotor::isCurrentLimited() const
{
    return currentLimited;
}

double ServoMotor::getResistance() const { return resistance; }
double ServoMotor::getInductance() const { return inductance; }
double ServoMotor::getTorqueConstant() const { return torqueConstant; }
double ServoMotor::getBackEmfConstant() const { return backEmfConstant; }
double ServoMotor::getRotorInertia() const { return rotorInertia; }
double ServoMotor::getViscousFriction() const { return viscousFriction; }
double ServoMotor::getCoulombFriction() const { return coulombFriction; }
double ServoMotor::getGearRatio() const { return gearRatio; }
double ServoMotor::getGearEfficiency() const { return gearEfficiency; }
double ServoMotor::getMaxCurrent() const { return maxCurrent; }
