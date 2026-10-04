#include "Comparator.h"

Comparator::Comparator() : lastError(0.0) {}

double Comparator::compare(double setpoint, double measurement) {
    lastError = setpoint - measurement;
    return lastError;
}

double Comparator::getLastError() {
    return lastError;
}

void Comparator::reset() {
    lastError = 0.0;
}
