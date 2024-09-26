// MODEL

#include <algorithm>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "Data.h"
#include "Screen.h"

void Data::getMenu() {
    menuView(banner, commandList, prompt);
}


void Data::exitOut() {
    if (currentScreen != "menuView") {
        getMenu();
    } else {
        exit(0);
    }
}


// Get Specific Process through ProcessName
void Data::getProcess(const std::string& processName) {
    auto it = std::find_if(processList.begin(), processList.end(), [&](ProcessInfo& process) {
        return process.processName == processName;
    });

    if (it != processList.end()) {
        rsScreenView(*it);
    } else { 
        std::cout << "ERROR: Process '" << processName << "' not found. Enter valid processes." << std::endl; 
    }
}


// Create a new Process
void Data::createProcess(const std::string& processName) {
    auto it = std::find_if(processList.begin(), processList.end(), [&](ProcessInfo& process) {
        return process.processName == processName;
    });

    if (it == processList.end()) {
        processList.emplace_back(processName, 1, 100, getTimestamp());
        rsScreenView(processList.back());
    } else {
        std::cout << "ERROR: Process with the name '" << processName << "' already exists." << std::endl;
    }
}


// List All Processes
void Data::listAllProcess() {
    return;
}


// Update a process
//ProcessInfo& Data::updateProcess() {
//    return ;
//}


// Get Timestamp
std::string Data::getTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm ltm{};
    localtime_s(&ltm, &now);
    std::stringstream ss;
    ss << std::put_time(&ltm, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}