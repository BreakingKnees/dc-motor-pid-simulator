#ifndef COMPARATOR_H
#define COMPARATOR_H

class Comparator {
private:
    double lastError;

public:
    Comparator();

    double compare(double setpoint, double measurement);
    double getLastError();
    void reset();
};

#endif
