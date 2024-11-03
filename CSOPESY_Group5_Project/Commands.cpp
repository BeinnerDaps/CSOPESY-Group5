#include "Commands.h"
#include "Data.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Commands::Commands() : data(), scheduler() { // Initialize data and scheduler
    scheduler.start();
}

void Commands::initialize() {
    for (int i = 1; i <= 10; ++i) {
        data.createProcess("process" + std::to_string(i));
    }
}

void Commands::processCommand(const std::string& command) {
    if (command.find("screen") != std::string::npos) {
        screenCommand(command);
    }
    else if (command == "scheduler-test") {
        schedulerTestCommand();
    }
    else if (command == "scheduler-stop") {
        schedulerStopCommand();
    }
    else if (command == "report-util") {
        reportUtilCommand();
    }
    else if (command == "clear") {
        clearScreen();
        menuView();
    }
    else if (command == "exit") {
        std::cout << "Terminating Serial OS, Thank you!" << std::endl;
        exit(0);
    }
    else {
        std::cout << "ERROR: Unrecognized command." << std::endl;
    }
}

void Commands::screenCommand(const std::string& command) {
    std::string subCommand, name;
    std::vector<std::string> subCommands = { "-r", "-s", "-ls" };

    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;

    if (subCommand != "-ls" && name.empty()) {
        std::cout << "ERROR: Process Not Specified" << std::endl;
        return;
    }

    auto it = std::find(subCommands.begin(), subCommands.end(), subCommand);
    int found = (it != subCommands.end()) ? std::distance(subCommands.begin(), it) : -1;

    switch (found) {
    case 0: rSubCommand(name); break;
    case 1: sSubCommand(name); break;
    case 2: lsSubCommand(); break;
    default: std::cout << "ERROR: Invalid Subcommand" << std::endl; break;
    }
}

void Commands::rSubCommand(const std::string& name) {
    clearScreen();
    std::cout << "Executing screen -r " << name << std::endl;
    data.createProcess(name);
    ProcessInfo& process = data.getProcess(name);
    displayProcess(process);
}

void Commands::sSubCommand(const std::string& name) {
    std::cout << "Executing screen -s " << name << std::endl;
    try {
        ProcessInfo& process = data.getProcess(name);
        lsScreenView(process);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Commands::lsSubCommand() {
    std::cout << "Waiting Queue:" << std::endl;
    auto runningProcesses = scheduler.getRunningProcesses();
    auto finishedProcesses = scheduler.getFinishedProcesses();

    // Display processes in the queue
    for (const auto& process : data.getProcessList()) {  // Use const auto& to avoid uninitialized reference
        auto isRunning = std::find_if(runningProcesses.begin(), runningProcesses.end(),
            [&](const ProcessInfo& p) { return p.processName == process.processName; });
        auto isFinished = std::find_if(finishedProcesses.begin(), finishedProcesses.end(),
            [&](const std::pair<ProcessInfo, int>& p) { return p.first.processName == process.processName; });

        if (isRunning == runningProcesses.end() && isFinished == finishedProcesses.end()) {
            std::cout << process.processName << "\tProcess created at: " << process.timeStamp
                << "\tCore: N/A\t" << process.currentLine << " / " << process.totalLine << std::endl;
        }
    }

    // Display running processes
    std::cout << "\nRunning processes:" << std::endl;
    for (const auto& process : runningProcesses) {
        std::cout << process.processName << "\tProcess created at: " << process.timeStamp
            << "\tCore: Active\t" << process.currentLine << " / " << process.totalLine << std::endl;
    }

    // Display finished processes
    std::cout << "\nFinished processes:" << std::endl;
    for (const auto& entry : finishedProcesses) {
        const auto& process = entry.first;
        int coreId = entry.second;
        std::cout << process.processName << "\tProcess created at: " << process.timeStamp
            << "\tFinished\tCore: " << coreId
            << "\t" << process.totalLine << " / " << process.totalLine << std::endl;
    }
}


void Commands::schedulerTestCommand() {
    std::cout << "Scheduling 10 Processes on 4 CPU Cores (Check via screen -ls)" << std::endl;

    initialize();

    for (auto& process : data.processList) {
        scheduler.addProcess(process);
    }
}

void Commands::schedulerStopCommand() {
    scheduler.stop();
}

void Commands::reportUtilCommand() {
    std::cout << "report-util command recognized. Reporting utilization." << std::endl;
}

void Commands::displayProcess(const ProcessInfo& process) {
    std::cout << "Displaying Process Details:" << std::endl;
    std::cout << "Process Name: " << process.processName << std::endl;
    std::cout << "Current Line: " << process.currentLine << std::endl;
    std::cout << "Total Lines: " << process.totalLine << std::endl;
    std::cout << "Process created at: " << process.timeStamp << std::endl;
    std::cout << "Status: " << (process.isFinished ? "Finished" : "Running") << std::endl;
}
