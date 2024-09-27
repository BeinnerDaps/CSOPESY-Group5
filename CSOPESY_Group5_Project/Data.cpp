// MODEL

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

#include "Data.h"

ProcessInfo& Data::getProcess(const std::string& processName){
    for (auto& process : processList) {
        if (process.processName == processName) { return process; }
    }
    std::cout << "ERROR: Process '" << processName << "' not found." << std::endl;
}

void Data::createProcess(const std::string& processName, int currentLine, int totalLine, const std::string& timeStamp) {
    for (const auto& process : processList) {
        if (process.processName == processName) {
            std::cout << "ERROR: Process with the name '" << processName << "' already exists." << std::endl;
            return;
        }
    }
    ProcessInfo newProcess = { processName, currentLine, totalLine, timeStamp };
    processList.push_back(newProcess);
}

void Data::listAllProcess() {

}

ProcessInfo& Data::updateProcess() {
    return;
}


std::string Data::getTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm ltm{};
    localtime_s(&ltm, &now);
    std::stringstream ss;
    ss << std::put_time(&ltm, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}