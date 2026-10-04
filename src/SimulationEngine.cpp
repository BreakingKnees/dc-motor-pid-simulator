#include "SimulationEngine.h"

using namespace std;

SimulationEngine::SimulationEngine(
    unique_ptr<Motor> motor,
    unique_ptr<ControllerComponent> controller,
    unique_ptr<Sensor> sensor,
    ThreadSafeQueue<LogRecord>& queue,
    const SimulationConfig& config
)
    : motor_(move(motor)),
      controller_(move(controller)),
      sensor_(move(sensor)),
      queue_(queue),
      config_(config),
      currentTime_(0.0),
      stepCount_(0)
{
}

void SimulationEngine::step()
{
    double dt = config_.getTimeStep();
    double targetSpeed = config_.getTargetSpeed();
    
    double trueSpeed = motor_->getAngularVelocity();
    double measuredSpeed = sensor_->measure(trueSpeed);
    
    double error = targetSpeed - measuredSpeed;
    double controlVoltage = controller_->compute(error, dt);
    
    double loadTorque = config_.getLoadTorque();
    motor_->setLoadTorque(loadTorque);
    motor_->step(controlVoltage, dt);

    LogRecord record;
    record.time = currentTime_;
    record.targetSpeed = targetSpeed;
    record.trueSpeed = motor_->getAngularVelocity();
    record.measuredSpeed = measuredSpeed;
    record.error = error;
    record.controlVoltage = controlVoltage;
    record.current = motor_->getCurrent();
    record.loadTorque = motor_->getLoadTorque();

    queue_.push(record);

    currentTime_ += dt;
    stepCount_++;
}

void SimulationEngine::run()
{
    while (currentTime_ <= config_.getDuration())
    {
        step();
    }
    
    queue_.close();
}

double SimulationEngine::getCurrentTime() const
{
    return currentTime_;
}

size_t SimulationEngine::getStepCount() const
{
    return stepCount_;
}
