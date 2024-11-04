// MODEL - Data.h

#ifndef DATA_H
#define DATA_H

#include <algorithm>
#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Data {

private:

    int pid = 1000;

public:
    Data() {}

    // Struct containing process information
    struct ProcessInfo {
        string processName;
        string arrivalTime;
        int pid;
        int currentLine;
        int totalLine;
        bool isFinished;
    };

    // List of processes
    vector<ProcessInfo> processList;

    // Method to get a specific process
    optional<ProcessInfo> getProcess(const string& processName) const;

    // Method to create a new process
    optional<ProcessInfo> createProcess(const string& processName);

    // Method to list all processes
    const vector<ProcessInfo>& listAllProcess() const;

    // Method to update processes
    void updateProcess(const int processcount);

    int getRandomNumber(int floor, int ceil);

    // Method to get current time
    string getTime();
};

#endif