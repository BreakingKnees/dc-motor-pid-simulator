#include <iostream>
#include <thread>
#include "SimulationConfig.h"
#include "ComponentFactory.h"
#include "SimulationEngine.h"
#include "LogConsumer.h"
#include "ThreadSafeQueue.h"
#include "LogRecord.h"

int main(int argc, char* argv[]) {
    // 1. Build SimulationConfig
    SimulationConfig config = SimulationConfig::fromArguments(argc, argv);
    
    if (config.wantsHelp()) {
        std::cout << SimulationConfig::getUsage() << std::endl;
        return 0;
    }
    
    config.validate();

    // 2. Create the shared thread-safe queue with max capacity
    ThreadSafeQueue<LogRecord> queue(1000);

    // 3. Create components using the factory
    auto motor = ComponentFactory::createMotor(config);
    auto controller = ComponentFactory::createController(config);
    auto sensor = ComponentFactory::createSensor(config);
    auto logger = ComponentFactory::createLogger(config);

    // 4. Inject them into the producer and consumer
    SimulationEngine engine(std::move(motor), std::move(controller), std::move(sensor), queue, config);
    LogConsumer consumer(queue, std::move(logger));

    // 5. Manage starting/joining the std::thread for the consumer
    std::thread consumerThread(&LogConsumer::run, &consumer);

    std::cout << "Starting simulation..." << std::endl;
    
    // Run producer on the main thread
    engine.run();
    
    std::cout << "Simulation finished. Waiting for consumer to finish logging..." << std::endl;

    // Join consumer thread
    consumerThread.join();
    
    std::cout << "Done. Records written: " << consumer.getRecordsWritten() << std::endl;

    return 0;
}
