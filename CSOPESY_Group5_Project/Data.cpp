// MODEL

#include "Data.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

ProcessInfo& Data::getProcess(const std::string& processName) {
    for (auto& process : processList) {
        if (process.processName == processName) {
            return process;
        }
    }
    throw std::runtime_error("ERROR: Process '" + processName + "' not found.");
}

void Data::createProcess(const std::string& processName) {
    std::string timeStamp = getTimestamp();
    int arrivalIndex = processList.size() + 1;
    ProcessInfo newProcess = {processName, 0, 100, timeStamp, arrivalIndex, false };
    processList.push_back(newProcess);
}

void Data::listAllProcess() {
    for (const auto& process : processList) {
        std::cout << "Process Name: " << process.processName << ", Status: "
            << (process.isFinished ? "Finished" : "Running")
            << ", Timestamp: " << process.timeStamp << std::endl;
    }
}

std::string Data::getTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm ltm{};
    #ifdef _WIN32
        localtime_s(&ltm, &now);
    #else
        localtime_r(&now, &ltm);
    #endif
        std::stringstream ss;
        ss << std::put_time(&ltm, "%m/%d/%Y, %I:%M:%S %p");
        return ss.str();
}
