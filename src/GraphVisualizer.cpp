#include "GraphVisualizer.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

GraphVisualizer::GraphVisualizer(const string& csvFilePath, const string& scriptPath)
    : csvFilePath_(csvFilePath),
      scriptPath_(scriptPath)
{
}

bool GraphVisualizer::plot() const
{
    // Check if the CSV file exists before attempting to plot
    ifstream fileCheck(csvFilePath_);
    if (!fileCheck.good())
    {
        cerr << "[GraphVisualizer Error]: Output file not found: " << csvFilePath_ << endl;
        return false;
    }
    fileCheck.close();

    cout << "Triggering automated graph plotting via Python..." << endl;

    string command = "python3 " + scriptPath_ + " \"" + csvFilePath_ + "\"";
    int returnCode = system(command.c_str());

    if (returnCode != 0)
    {
        cerr << "[GraphVisualizer Warning]: Plotting script exited with code " << returnCode << endl;
        return false;
    }

    return true;
}
