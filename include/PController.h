#ifndef PCONTROLLER_H
#define PCONTROLLER_H

#include "Gain.h"
#include <memory>

class PController : public Gain {
public:
    PController(double kp);

    std::unique_ptr<ControllerComponent> clone() override;
};

#endif
