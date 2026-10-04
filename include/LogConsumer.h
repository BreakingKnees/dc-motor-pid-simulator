#pragma once
#include <memory>
#include <cstddef>
#include "ThreadSafeQueue.h"
#include "LogRecord.h"
#include "OutputLogger.h"

class LogConsumer {
private:
    ThreadSafeQueue<LogRecord>& queue_;
    std::unique_ptr<OutputLogger> logger_;
    std::size_t recordsWritten_;

public:
    LogConsumer(ThreadSafeQueue<LogRecord>& queue, std::unique_ptr<OutputLogger> logger);
    void run();
    std::size_t getRecordsWritten() const;
};
