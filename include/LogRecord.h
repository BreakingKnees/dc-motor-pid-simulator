#pragma once

struct LogRecord {
    double time;
    double targetSpeed;
    double trueSpeed;
    double measuredSpeed;
    double error;
    double controlVoltage;
    double current;
    double loadTorque;
};
