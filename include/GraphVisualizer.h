#pragma once
#include <string>

using namespace std;

class GraphVisualizer {
private:
    string csvFilePath_;
    string scriptPath_;

public:
    GraphVisualizer(const string& csvFilePath, const string& scriptPath = "plot.py");

    bool plot() const;
};
