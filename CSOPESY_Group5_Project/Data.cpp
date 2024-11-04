// MODEL - DATA

#include "Data.h"


// Get Specific Process through ProcessName
optional<Data::ProcessInfo> Data::getProcess(const string& name) const {
    auto it = find_if(processList.begin(), processList.end(), [&](const ProcessInfo& process) {
        return process.processName == name;
    });

    return (it != processList.end()) ? optional<ProcessInfo>(*it) : nullopt;
}


// Create a new Process
optional<Data::ProcessInfo> Data::createProcess(const string& processName) {
    auto it = find_if(processList.begin(), processList.end(), [&](ProcessInfo& process) {
        return process.processName == processName;
    });

    if (it == processList.end()) {
        processList.emplace_back(processName, getTime(), ++pid, 0, getRandomNumber(100, 5000), false);
        updateProcess(processList.size());
        return processList.back();
    } else {
        return nullopt;
    }
}


// List All Processes
const vector<Data::ProcessInfo>& Data::listAllProcess() const {
    return processList;
}

// Update a process
void Data::updateProcess(const int processcount) {
    for (auto& process : processList) { process.totalLine = processcount; }
}


// Create a random value for totalLine
int Data::getRandomNumber(int floor, int ceil) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(floor, ceil);
    return distrib(gen);
}


// Get Timestamp
string Data::getTime() {
    time_t now = time(nullptr);
    tm ltm{};
    localtime_s(&ltm, &now);

    stringstream ss;
    ss << put_time(&ltm, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}