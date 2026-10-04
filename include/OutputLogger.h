#pragma once

#include "LogRecord.h"

class OutputLogger {
public:
    virtual ~OutputLogger() = default;

    virtual void open() = 0;
    virtual void log(const LogRecord& record) = 0;
    virtual void flush() = 0;
    virtual void close() = 0;
    virtual bool isOpen() = 0;
};
