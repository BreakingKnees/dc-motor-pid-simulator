#pragma once

#include <cstddef>
#include <fstream>
#include <string>

#include "OutputLogger.h"

class CSVOutput : public OutputLogger {
public:
    CSVOutput(std::string filePath, int precision);
    ~CSVOutput() override;

    void open() override;
    void log(const LogRecord& record) override;
    void flush() override;
    void close() override;
    bool isOpen() override;

    std::size_t getRowsWritten();

private:
    std::string filePath_;
    int precision_;
    std::size_t rowsWritten_;
    bool isOpen_;
    std::ofstream fileStream_;
};
