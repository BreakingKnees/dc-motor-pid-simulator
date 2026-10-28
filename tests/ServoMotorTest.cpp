#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "Motor.h"
#include "ServoMotor.h"

static int failures = 0;

#define CHECK(cond)                                                      \
    do {                                                                 \
        if (!(cond)) {                                                   \
            std::cerr << "FAIL line " << __LINE__ << ": " #cond "\n";    \
            ++failures;                                                  \
        }                                                                \
    } while (0)

// Same defaults as ComponentFactory::createMotor for --motor servo.
static ServoMotor makeServo(double maxCurrent = 5.0, double gearRatio = 4.0)
{
    return ServoMotor(1.0, 0.005, 0.02, 0.02, 5e-5, 1e-5, 2e-3,
                      gearRatio, 0.9, maxCurrent, 0.0);
}

template <typename F>
static bool throwsInvalid(F f)
{
    try { f(); } catch (const std::invalid_argument&) { return true; }
    return false;
}

static void run(ServoMotor& m, double volts, double seconds, double dt)
{
    for (double t = 0.0; t < seconds; t += dt) m.step(volts, dt);
}

int main()
{
    // Usable through the abstract Motor interface (Liskov compatibility).
    std::unique_ptr<Motor> base(new ServoMotor(makeServo()));
    CHECK(base->getName() == "ServoMotor");
    base->step(6.0, 0.01);
    CHECK(base->getAngularVelocity() > 0.0);

    // Steady state with 6 V, no load: analytic output speed.
    // 0 = Kt*i - b*w - Tc*tanh(w) with i = (V - Ke*w)/R. Solve numerically
    // by simply checking the simulation settles and is self-consistent.
    {
        ServoMotor m = makeServo();
        run(m, 6.0, 3.0, 0.01);
        double wr = m.getRotorVelocity();
        double i = (6.0 - 0.02 * wr) / 1.0;
        double residual = 0.02 * i - 1e-5 * wr - 2e-3 * std::tanh(wr);
        CHECK(std::fabs(residual) < 1e-4);
        CHECK(std::fabs(m.getRotorVelocity() - 4.0 * m.getAngularVelocity()) < 1e-9);
        CHECK(!m.isCurrentLimited());
    }

    // Stable with a coarse 10 ms step (electrical tau is 5 ms).
    {
        ServoMotor m = makeServo();
        run(m, 12.0, 5.0, 0.01);
        CHECK(std::isfinite(m.getAngularVelocity()));
        CHECK(m.getAngularVelocity() < 200.0);
    }

    // Current limit: stall-like start at full voltage saturates current.
    {
        ServoMotor m = makeServo(2.0);
        m.step(12.0, 0.01);
        CHECK(m.isCurrentLimited());
        CHECK(std::fabs(m.getCurrent()) <= 2.0 + 1e-12);
        run(m, -12.0, 2.0, 0.01);
        CHECK(std::fabs(m.getCurrent()) <= 2.0 + 1e-12);
    }

    // Load torque on the output shaft slows the motor; more gearing helps.
    {
        ServoMotor free = makeServo();
        ServoMotor loaded = makeServo();
        loaded.setLoadTorque(0.1);
        run(free, 8.0, 3.0, 0.01);
        run(loaded, 8.0, 3.0, 0.01);
        CHECK(loaded.getAngularVelocity() < free.getAngularVelocity());
    }

    // Position integrates output velocity.
    {
        ServoMotor m = makeServo();
        run(m, 6.0, 3.0, 0.01);
        CHECK(m.getAngularPosition() > 0.0);
    }

    // Reset restores the initial state.
    {
        ServoMotor m = makeServo();
        m.setLoadTorque(0.2);
        run(m, 6.0, 1.0, 0.01);
        m.reset();
        CHECK(m.getAngularVelocity() == 0.0);
        CHECK(m.getAngularPosition() == 0.0);
        CHECK(m.getCurrent() == 0.0);
        CHECK(m.getLoadTorque() == 0.0);
        CHECK(!m.isCurrentLimited());
    }

    // Invalid parameters are rejected.
    CHECK(throwsInvalid([] { ServoMotor(0, 0.005, 0.02, 0.02, 5e-5, 0, 0, 4, 0.9, 5, 0); }));
    CHECK(throwsInvalid([] { ServoMotor(1, 0.0,   0.02, 0.02, 5e-5, 0, 0, 4, 0.9, 5, 0); }));
    CHECK(throwsInvalid([] { ServoMotor(1, 0.005, 0.02, 0.02, 5e-5, 0, 0, 0, 0.9, 5, 0); }));
    CHECK(throwsInvalid([] { ServoMotor(1, 0.005, 0.02, 0.02, 5e-5, 0, 0, 4, 1.5, 5, 0); }));
    CHECK(throwsInvalid([] { ServoMotor(1, 0.005, 0.02, 0.02, 5e-5, 0, 0, 4, 0.9, 0, 0); }));
    CHECK(throwsInvalid([] { ServoMotor(1, 0.005, 0.02, 0.02, 5e-5, -1, 0, 4, 0.9, 5, 0); }));

    if (failures == 0) std::cout << "All ServoMotor tests passed\n";
    return failures == 0 ? 0 : 1;
}
