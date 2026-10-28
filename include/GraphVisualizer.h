#pragma once
#include <string>

class GraphVisualizer {
private:
    std::string csvFilePath_;
    std::string scriptPath_;

public:
    GraphVisualizer(const std::string& csvFilePath, const std::string& scriptPath = "plot.py");

    bool plot() const;
};
