// MODEL

#include "Data.h"


// Get Specific Process through ProcessName
std::optional<Data::ProcessInfo> Data::getProcess(const std::string& name) const {
    auto it = std::find_if(processList.begin(), processList.end(), [&](const ProcessInfo& process) {
        return process.processName == name;
    });

    return (it != processList.end()) ? std::optional<ProcessInfo>(*it) : std::nullopt;
}


// Create a new Process
std::optional<Data::ProcessInfo> Data::createProcess(const std::string& name) {
    auto it = std::find_if(processList.begin(), processList.end(), [&](ProcessInfo& process) {
        return process.processName == name;
    });

    if (it == processList.end()) {
        processList.emplace_back(name, processList.size() + 1, processList.size() + 1, getTime());
        updateProcess(processList.size());
        return processList.back();
    } else {
        return std::nullopt;
    }
}


// List All Processes
const std::vector<Data::ProcessInfo>& Data::listAllProcess() const {
    return processList;
}


// Update a process
void Data::updateProcess(const int processcount) {
    for (auto& process : processList) { process.totalLine = processcount; }
}


// Get Timestamp
std::string Data::getTime() {
    std::time_t now = std::time(nullptr);
    std::tm ltm{};
    localtime_s(&ltm, &now);
    std::stringstream ss;
    ss << std::put_time(&ltm, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}