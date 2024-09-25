// Data.h
#ifndef DATA_H
#define DATA_H

#include <string>

#include "Screen.h"

struct ProcessInfo {
    std::string processName;
    int currentLine;
    int totalLine;
    std::string timeStamp;
};

class Data {

protected:

    std::vector<ProcessInfo> processList;

    Data() {}

    // Method to display a specific process
    ProcessInfo& getProcess(const std::string& processName);

    // Method to create a new process
    void createProcess(const std::string& processName, int currentLine, int totalLine, const std::string& timeStamp);

    // Method to list all processes
    void listAllProcess(); 

    ProcessInfo& updateProcess();

    // Get current Timestamp
    std::string getTimestamp() const;
};

#endif