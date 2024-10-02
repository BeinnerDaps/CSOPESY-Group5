// Data.h
#ifndef DATA_H
#define DATA_H


#include <algorithm>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>


class Data {

public:
    Data() {}
    
    // Struct containing process information
    struct ProcessInfo {
        std::string processName;
        int currentLine;
        int totalLine;
        std::string timeStamp;
    };

    // List of processes
    std::vector<ProcessInfo> processList;

    // Method to get a specific process
    std::optional<ProcessInfo> getProcess(const std::string& processName) const;

    // Method to create a new process
    std::optional<ProcessInfo> createProcess(const std::string& processName);

    // Method to list all processes
    const std::vector<ProcessInfo>& listAllProcess() const;

    // Method to update processes
    void updateProcess(const int processcount);

    // Method to get current time
    std::string getTime();
};

#endif