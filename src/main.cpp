#include <iostream>
#include <thread>
#include "SimulationConfig.h"
#include "ComponentFactory.h"
#include "SimulationEngine.h"
#include "LogConsumer.h"
#include "ThreadSafeQueue.h"
#include "LogRecord.h"
#include "GraphVisualizer.h"

using namespace std;

int main(int argc, char* argv[]) {
    // 1. Build SimulationConfig
    SimulationConfig config = SimulationConfig::fromArguments(argc, argv);
    
    if (config.wantsHelp()) {
        cout << SimulationConfig::getUsage() << endl;
        return 0;
    }
    
    try
    {
        config.validate();
    }
    catch (const exception& e)
    {
        cerr << "Configuration Error: " << e.what() << endl;
        return 1;
    }

    // 2. Create the shared thread-safe queue with max capacity
    ThreadSafeQueue<LogRecord> queue(1000);

    // 3. Create components using the factory
    auto motor = ComponentFactory::createMotor(config);
    auto controller = ComponentFactory::createController(config);
    auto sensor = ComponentFactory::createSensor(config);
    auto logger = ComponentFactory::createLogger(config);

    // 4. Inject them into the producer and consumer
    SimulationEngine engine(move(motor), move(controller), move(sensor), queue, config);
    LogConsumer consumer(queue, move(logger));

    // 5. Manage starting/joining the thread for the consumer
    thread consumerThread(&LogConsumer::run, &consumer);

    cout << "Starting simulation..." << endl;
    
    // Run producer on the main thread
    engine.run();
    
    cout << "Simulation finished. Waiting for consumer to finish logging..." << endl;

    // Join consumer thread
    consumerThread.join();
    
    cout << "Done. Records written: " << consumer.getRecordsWritten() << endl;

    // Automated visualization
    GraphVisualizer visualizer(config.getOutputFile());
    visualizer.plot();

    return 0;
}
