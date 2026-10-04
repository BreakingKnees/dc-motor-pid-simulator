#include "CSVOutput.h"

#include <iomanip>
#include <utility>

CSVOutput::CSVOutput(std::string filePath, int precision)
    : filePath_(std::move(filePath)),
      precision_(precision),
      rowsWritten_(0),
      isOpen_(false) {
}

CSVOutput::~CSVOutput() {
    if (isOpen_) {
        close();
    }
}

void CSVOutput::open() {
    fileStream_.open(filePath_);

    if (fileStream_.is_open()) {
        fileStream_ << std::setprecision(precision_);
        fileStream_
            << "time,targetSpeed,trueSpeed,measuredSpeed,error,"
               "controlVoltage,current,loadTorque\n";

        isOpen_ = true;
        rowsWritten_ = 0;
    } else {
        isOpen_ = false;
    }
}

void CSVOutput::log(const LogRecord& record) {
    if (isOpen_) {
        fileStream_ << record.time << ","
                    << record.targetSpeed << ","
                    << record.trueSpeed << ","
                    << record.measuredSpeed << ","
                    << record.error << ","
                    << record.controlVoltage << ","
                    << record.current << ","
                    << record.loadTorque << "\n";
        ++rowsWritten_;
    }
}

void CSVOutput::flush() {
    if (isOpen_) {
        fileStream_.flush();
    }
}

void CSVOutput::close() {
    if (isOpen_) {
        fileStream_.close();
        isOpen_ = false;
    }
}

bool CSVOutput::isOpen() {
    return isOpen_;
}

std::size_t CSVOutput::getRowsWritten() {
    return rowsWritten_;
}
