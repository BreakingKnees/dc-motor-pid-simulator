#include "LogConsumer.h"

LogConsumer::LogConsumer(ThreadSafeQueue<LogRecord>& queue, std::unique_ptr<OutputLogger> logger)
    : queue_(queue), logger_(std::move(logger)), recordsWritten_(0)
{
}

void LogConsumer::run()
{
    logger_->open();
    
    LogRecord record;
    while (queue_.waitAndPop(record))
    {
        logger_->log(record);
        recordsWritten_++;
    }
    
    logger_->close();
}

std::size_t LogConsumer::getRecordsWritten() const
{
    return recordsWritten_;
}
