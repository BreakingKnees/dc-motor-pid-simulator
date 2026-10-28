#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "Motor.h"
#include <string>

// Geared, current-limited permanent-magnet servo motor.
//
// Compared with BrushedDCMotor this model adds the things that make a servo
// behave like a servo in a speed loop:
//   * a gearbox (gear ratio N, efficiency eta) between rotor and output shaft
//   * a drive current limit (peak current), so torque saturates
//   * Coulomb (static-type) friction on top of viscous friction
//
// Conventions (so the model stays drop-in compatible with Motor):
//   * getAngularVelocity()/getAngularPosition() report the OUTPUT shaft,
//     which is what the tachometer in the simulation measures.
//   * setLoadTorque() is the torque applied to the OUTPUT shaft.
//   * getCurrent() is the armature current (after the current limit).
class ServoMotor : public Motor {
public:
    ServoMotor(
        double resistance,        // ohm            (> 0)
        double inductance,        // H              (> 0)
        double torqueConstant,    // N*m/A          (> 0)
        double backEmfConstant,   // V*s/rad        (> 0)
        double rotorInertia,      // kg*m^2, rotor  (> 0)
        double viscousFriction,   // N*m*s/rad, rotor side (>= 0)
        double coulombFriction,   // N*m, rotor side       (>= 0)
        double gearRatio,         // rotor turns per output turn (> 0)
        double gearEfficiency,    // (0, 1]
        double maxCurrent,        // A, drive peak current (> 0)
        double initialVelocity    // rad/s, output shaft
    );

    void step(double appliedVoltage, double timeStep) override;
    void reset() override;
    std::string getName() const override;

    double getBackEmf() const;
    double getElectromagneticTorque() const;   // rotor shaft, Kt * i
    double getOutputTorque() const;            // output shaft, N * eta * Kt * i
    double getRotorVelocity() const;           // rad/s, = N * output velocity
    bool   isCurrentLimited() const;           // true if limit was active last step

    double getResistance() const;
    double getInductance() const;
    double getTorqueConstant() const;
    double getBackEmfConstant() const;
    double getRotorInertia() const;
    double getViscousFriction() const;
    double getCoulombFriction() const;
    double getGearRatio() const;
    double getGearEfficiency() const;
    double getMaxCurrent() const;

private:
    double resistance;
    double inductance;
    double torqueConstant;
    double backEmfConstant;
    double rotorInertia;
    double viscousFriction;
    double coulombFriction;
    double gearRatio;
    double gearEfficiency;
    double maxCurrent;

    double maxSubStep;  // largest stable internal Euler step (seconds)

    double backEmf;
    double electromagneticTorque;
    bool   currentLimited;
};

#endif
