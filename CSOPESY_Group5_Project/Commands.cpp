// CONTROLLER

#include <iostream>
#include <vector>
#include <sstream>
#include <functional>
#include <unordered_map>

#include "Commands.h"
#include "Data.h"

void Commands::processCommand(const std::string& command) {
    if (command.find("screen") != std::string::npos) {
        screenCommand(command);
    } else if (command == "scheduler-test") {
        schedulerTestCommand();
    } else if (command == "scheduler-stop") {
        schedulerStopCommand();
    } else if (command == "report-util") {
        reportUtilCommand();
    } else if (command == "clear") {
        getMenu();
    } else if (command == "exit") {
        exitOut();
    } else {
        std::cout << "ERROR: Unrecognized command." << std::endl;
    }
}

void Commands::screenCommand(const std::string& command) {
    std::string subCommand, name;

    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;

    if (subCommand != "-ls" && name.empty()) {
        std::cout << "ERROR: Process Not Specified" << std::endl;
        return;
    }

    const std::unordered_map<std::string, std::function<void(const std::string&)>> subCommandActions = {
        { "-r", [this](const std::string& name) { createProcess(name); } }, // -r creates a new process
        { "-s", [this](const std::string& name) { getProcess(name); } },    // -s gets existing process
        { "-ls", [this](const std::string&) { listAllProcess(); } }         // -ls lists all processes
    };

    auto it = subCommandActions.find(subCommand);

    if (it == subCommandActions.end()) {
        std::cout << "ERROR: Invalid Subcommand" << std::endl;
        return;
    }

    it->second(name);   
}

void Commands::schedulerTestCommand() {
    std::cout << "scheduler-test command recognized. Doing something." << std::endl;
}

void Commands::schedulerStopCommand() {
    std::cout << "scheduler-stop command recognized. Doing something." << std::endl;
}

void Commands::reportUtilCommand() {
    std::cout << "report-util command recognized. Doing something." << std::endl;
}