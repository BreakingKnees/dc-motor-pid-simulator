// Standalone test for the dynamic noise configuration (Ryan's classes).
//
// Build & run from the project root:
//   g++ -std=c++14 -Wall -Wextra -Iinclude tests/NoiseConfigTest.cpp
//       src/ZeroNoise.cpp src/GaussianNoise.cpp src/UniformNoise.cpp
//       src/TachometerSensor.cpp -o build/noise_test   (one line)
//   ./build/noise_test
//
// Lives in tests/ (not src/) so the Makefile's src/*.cpp wildcard does not
// pull a second main() into the simulator.

#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>

#include "GaussianNoise.h"
#include "TachometerSensor.h"
#include "UniformNoise.h"
#include "ZeroNoise.h"

static int failures = 0;

#define CHECK(cond)                                                      \
    do {                                                                 \
        if (!(cond)) {                                                   \
            std::cerr << "FAIL line " << __LINE__ << ": " #cond "\n";    \
            ++failures;                                                  \
        }                                                                \
    } while (0)

template <typename F>
static bool throwsInvalid(F f) {
    try {
        f();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

int main() {
    // 1. Backward compatibility: same numbers as the plain std:: distributions
    {
        GaussianNoise g(1.5, 0.3, 42);
        std::mt19937 gen(42);
        std::normal_distribution<double> ref(1.5, 0.3);
        bool same = true;
        for (int i = 0; i < 1000; ++i) same = same && (g.sample() == ref(gen));
        CHECK(same);

        UniformNoise u(-2.0, 3.0, 7);
        std::mt19937 gen2(7);
        std::uniform_real_distribution<double> ref2(-2.0, 3.0);
        same = true;
        for (int i = 0; i < 1000; ++i) same = same && (u.sample() == ref2(gen2));
        CHECK(same);
    }

    // 2. Gaussian: change mean/stddev at runtime
    {
        GaussianNoise g(0.0, 1.0, 1);
        g.setMean(100.0);
        g.setStandardDeviation(0.0);
        CHECK(g.sample() == 100.0);          // stddev 0 -> constant = mean
        CHECK(g.setParameter("mean", -5.0));
        double v = 0;
        CHECK(g.getParameter("mean", v) && v == -5.0);
        CHECK(g.setParameter("stddev", 2.0));
        CHECK(g.getParameter("stddev", v) && v == 2.0);
    }

    // 3. Gaussian: statistics follow the new settings
    {
        GaussianNoise g(0.0, 1.0, 123);
        g.setMean(10.0);
        g.setStandardDeviation(0.5);
        double sum = 0, sumSq = 0;
        const int N = 200000;
        for (int i = 0; i < N; ++i) {
            double x = g.sample();
            sum += x;
            sumSq += x * x;
        }
        double mean = sum / N;
        double sd = std::sqrt(sumSq / N - mean * mean);
        CHECK(std::fabs(mean - 10.0) < 0.01);
        CHECK(std::fabs(sd - 0.5) < 0.01);
    }

    // 4. Gaussian: invalid values rejected, state unchanged
    {
        GaussianNoise g(1.0, 1.0, 1);
        CHECK(throwsInvalid([&] { g.setStandardDeviation(-0.1); }));
        CHECK(throwsInvalid([&] { g.setMean(NAN); }));
        CHECK(throwsInvalid([] { GaussianNoise bad(0.0, -1.0, 1); }));
        CHECK(!g.setParameter("stddev", -3.0));
        CHECK(!g.setParameter("mean", INFINITY));
        CHECK(!g.setParameter("nonsense", 1.0));
        CHECK(!g.setParameter("seed", -1.0));
        CHECK(!g.setParameter("seed", 1.5));
        CHECK(g.getMean() == 1.0 && g.getStandardDeviation() == 1.0);
    }

    // 5. Uniform: bounds at runtime, samples stay inside
    {
        UniformNoise u(-1.0, 1.0, 5);
        u.setBounds(5.0, 10.0);
        bool inside = true;
        for (int i = 0; i < 10000; ++i) {
            double x = u.sample();
            inside = inside && x >= 5.0 && x <= 10.0;
        }
        CHECK(inside);
        CHECK(u.setParameter("upper", 20.0));
        CHECK(u.getUpperBound() == 20.0);
        u.setBounds(3.0, 3.0);
        CHECK(u.sample() == 3.0);            // degenerate range is safe
    }

    // 6. Uniform: invalid bounds rejected, state unchanged
    {
        UniformNoise u(-1.0, 1.0, 5);
        CHECK(throwsInvalid([&] { u.setBounds(2.0, 1.0); }));
        CHECK(throwsInvalid([&] { u.setLowerBound(5.0); }));  // > current upper
        CHECK(throwsInvalid([] { UniformNoise bad(1.0, -1.0, 1); }));
        CHECK(!u.setParameter("lower", 99.0));
        CHECK(!u.setParameter("lower", NAN));
        CHECK(u.getLowerBound() == -1.0 && u.getUpperBound() == 1.0);
    }

    // 7. Seed change restarts the stream; reset() uses the new seed
    {
        GaussianNoise a(0.0, 1.0, 1), b(0.0, 1.0, 2);
        a.setSeed(2);
        CHECK(a.sample() == b.sample());
        double first = a.sample();
        a.reset();
        a.sample();
        CHECK(a.sample() == first);
    }

    // 8. Changing mean mid-run does NOT restart the stream
    {
        GaussianNoise a(0.0, 1.0, 9), b(0.0, 1.0, 9);
        a.sample();
        b.sample();
        a.setMean(4.0);
        CHECK(a.sample() == b.sample() + 4.0);
    }

    // 9. ZeroNoise: not configurable, never crashes
    {
        ZeroNoise z;
        double v = 0;
        CHECK(!z.setParameter("mean", 1.0));
        CHECK(!z.getParameter("mean", v));
        CHECK(z.sample() == 0.0);
    }

    // 10. Through the sensor (the path the engine would use via Sensor*)
    {
        std::unique_ptr<Sensor> sensor(new TachometerSensor(
            std::unique_ptr<NoiseGenerator>(new GaussianNoise(0.0, 1.0, 3))));
        CHECK(sensor->configureNoise("mean", 50.0));
        CHECK(sensor->configureNoise("stddev", 0.0));
        CHECK(sensor->measure(100.0) == 150.0);
        CHECK(!sensor->configureNoise("lower", 0.0));   // not a Gaussian setting

        TachometerSensor* t = static_cast<TachometerSensor*>(sensor.get());
        double v = 0;
        CHECK(t->getNoiseParameter("mean", v) && v == 50.0);

        // Swapping generators at runtime still works (existing feature)
        t->setNoiseGenerator(
            std::unique_ptr<NoiseGenerator>(new UniformNoise(0.0, 0.0, 1)));
        CHECK(t->configureNoise("lower", -0.5));
        CHECK(t->configureNoise("upper", 0.5));

        TachometerSensor none(nullptr);
        CHECK(!none.configureNoise("mean", 1.0));
        CHECK(none.measure(7.0) == 7.0);
    }

    if (failures == 0) {
        std::cout << "All noise configuration tests passed.\n";
        return 0;
    }
    std::cerr << failures << " check(s) failed.\n";
    return 1;
}
