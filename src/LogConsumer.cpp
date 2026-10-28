#include "LogConsumer.h"

using namespace std;

LogConsumer::LogConsumer(ThreadSafeQueue<LogRecord>& queue, unique_ptr<OutputLogger> logger)
    : queue_(queue), logger_(move(logger)), recordsWritten_(0)
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

size_t LogConsumer::getRecordsWritten() const
{
    return recordsWritten_;
}
