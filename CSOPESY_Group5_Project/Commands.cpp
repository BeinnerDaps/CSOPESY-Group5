// CONTROLLER

#include <iostream>
#include <vector>
#include <sstream>

#include "Commands.h"
#include "Data.h"
#include "Screen.h"

void Commands::initialScreen() {
    clearScreen();
    menuView();
    currentScreen 
}

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
        clearScreen(); 
        menuView();
    } else if (command == "exit") {

        std::cout << "Terminating Serial OS, Thank you!" << std::endl;
        exit(0);
    } else {
        std::cout << "ERROR: Unrecognized command." << std::endl;
    }
}

void Commands::screenCommand(const std::string& command) {
    std::string subCommand, name;
    std::vector<std::string> subCommands = { "-r", "-s", "-ls"};

    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;

    if (name.empty()) {
        std::cout << "ERROR: Process Not Specified" << std::endl;
        return;
    }

    auto it = std::find(subCommands.begin(), subCommands.end(), subCommand);
    int found = (it != subCommands.end()) ? std::distance(subCommands.begin(), it) : -1;

    switch (found) {
    case 0: rSubCommand(name); break;
    case 1: sSubCommand(name); break;
    case 3: lsSubCommand(); break;
    default: std::cout << "ERROR: Invalid Subcommand" << std::endl; break;
    }
}

void Commands::rSubCommand(const std::string& name) {
    clearScreen();
    std::cout << "Executing screen -r " <<  name << std::endl;
    createProcess(name, 0, 100, getTimestamp());
    displayProcess(name);
}

void Commands::sSubCommand(const std::string& name) {
    std::cout << "Executing screen -s " << name << std::endl;
    try {
        ProcessInfo& process = getProcess(name);
        lsScreenView(process);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Commands::lsSubCommand() {
    
    
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