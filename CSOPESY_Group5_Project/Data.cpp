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
optional<Data::ProcessInfo> Data::createProcess(const string& name) {
    auto it = find_if(processList.begin(), processList.end(), [&](ProcessInfo& process) {
        return process.processName == name;
    });

    if (it == processList.end()) {
        processList.emplace_back(name, processList.size()+1, processList.size()+1, getTime());
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


// Get Timestamp
string Data::getTime() {
    time_t now = time(nullptr);
    tm ltm{};
    localtime_s(&ltm, &now);

    const char* daysOfWeek[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
    string day = daysOfWeek[ltm.tm_wday];

    stringstream ss;
    ss << day << ", " << std::put_time(&ltm, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}