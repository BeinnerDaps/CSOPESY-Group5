#include "Commands.h"
#include "ProcessInfo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <chrono>

static int nextProcessID = 1; // For unique process IDs

// Constructor
Commands::Commands() : scheduler(nullptr) {}

// 0. Initialize Program
void Commands::initialize(const std::string& filename) {
    if (scheduler == nullptr) {
        while (true) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error: Could not open the specified file. Please enter a valid path." << std::endl;
                continue;
            }
            file.close();

            try {
                config = parseConfigFile(filename);
                scheduler = std::make_unique<Scheduler>(config);
                std::cout << "Scheduler initialized with " << config.numCpu << " CPUs." << std::endl;
                break;
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing config file: " << e.what() << std::endl;
            }
        }
    }
}

// 0.1. Parse Config.txt File
Config Commands::parseConfigFile(const std::string& filename) {
    Config config;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (!(iss >> key)) continue;

        if (key == "num-cpu") iss >> config.numCpu;
        else if (key == "scheduler") iss >> config.scheduler;
        else if (key == "quantum-cycles") iss >> config.quantumCycles;
        else if (key == "batch-process-freq") iss >> config.batchProcessFreq;
        else if (key == "min-ins") iss >> config.minIns;
        else if (key == "max-ins") iss >> config.maxIns;
        else if (key == "delay-per-exec") iss >> config.delaysPerExec;
        else if (key == "max-overall-mem") iss >> config.overallMem;
        else if (key == "mem-per-frame") iss >> config.frameMem;
        else if (key == "min-mem-per-proc") iss >> config.procMinMem;
        else if (key == "max-mem-per-proc") iss >> config.procMaxMem;
    }

    if (!config.scheduler.empty() && config.scheduler.front() == '"' && config.scheduler.back() == '"') {
        config.scheduler = config.scheduler.substr(1, config.scheduler.size() - 2);
    }

    if (config.overallMem && config.frameMem) {
        config.numFrame = config.overallMem / config.frameMem;
    }

    file.close();
    return config;
}

// Process Commands
void Commands::processCommand(const std::string& input) {

    const std::unordered_map<std::string, std::function<void(const std::string&)>> CommandActions = {
        { "initialize", [this](const std::string& input) { initialize("config.txt"); } },
        { "marquee", [this](const std::string& input) { marqueeCommand(input); } },
        { "screen", [this](const std::string& input) { screenCommand(input); } },
        { "nvidia-smi", [this](const std::string&) { nvidsmiCommand(); } },
        { "scheduler-test", [this](const std::string&) { schedulerTestCommand(); } },
        { "scheduler-stop", [this](const std::string&) { schedulerStopCommand(); } },
        { "report-util", [this](const std::string&) {  reportUtilCommand(); } },
        { "clear", [this](const std::string&) {  menuView(); } },
        { "exit", [this](const std::string&) {  exitCommand(); } }
    };

    std::istringstream iss(input);
    std::string command;
    iss >> command;

    auto it = CommandActions.find(command);

    if (it == CommandActions.end()) {
        std::cout << "ERROR: Unrecognized Command" << std::endl;
        return;
    }

    it->second(input);
}

// 1. Marquee Command
void Commands::marqueeCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string text;
    std::string marq;
    int refresh = 0, poll = 0;
    iss >> marq >> text >> refresh >> poll;

    if (!refresh) { refresh = 50; } // Default refresh rate
    if (!poll) { poll = 50; } // Default poll rate
    if (text.empty()) { text = "threaded"; }

    Marquee marquee(refresh, poll, true);

    if (text == "threaded") { 
        marquee.startThread(); 
        marqueeView(); 
        menuView();
    } else if (text == "nonthreaded") { 
        marquee.startNonThread(); 
        marqueeView(); 
        menuView();
    } else { 
        std::cout << "ERROR: Incomplete parameters" << std::endl; 
    }
}

// 2. Screen Command
void Commands::screenCommand(const std::string& command) {
    std::string subCommand, name;
    std::vector<std::string> subCommands = { "-r", "-s", "-ls" };

    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;

    if (subCommand != "-ls" && name.empty()) {
        std::cout << "ERROR: Process Not Specified" << std::endl;
        return;
    }

    auto it = find(subCommands.begin(), subCommands.end(), subCommand);

    int found = (it != subCommands.end()) ? distance(subCommands.begin(), it) : -1;

    switch (found) {
    case 0: rSubCommand(name); break; // Reattach
    case 1: sSubCommand(name); break; // Start a new process
    case 2: writeProcessReport(std::cout); break; // List processes
    default: std::cout << "ERROR: Invalid Subcommand" << std::endl; break;
    }
}

// 2.1. Find Existing Process
void Commands::rSubCommand(const std::string& name) {
    clearScreen();
    std::cout << "Attempting to reattach to process: " << name << std::endl;

    try {
        ProcessInfo& process = scheduler->getProcess(name);
        enterProcessScreen(process);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

// 2.2. Create New Process
void Commands::sSubCommand(const std::string& name) {
    try {
        ProcessInfo& existingProcess = scheduler->getProcess(name);
        std::cout << "Reattaching to existing process: " << name << std::endl;
        enterProcessScreen(existingProcess);
    }
    catch (const std::runtime_error& e) {
        std::cout << "Creating new process \"" << name << "\"." << std::endl;

        ProcessInfo newProcess(
            nextProcessID++,
            name,
            getTimestamp(),
            scheduler->getRandomInt(config.procMinMem, config.procMaxMem),
            0,
            scheduler->getRandomInt(config.minIns, config.maxIns),
            false
        );

        scheduler->addProcess(newProcess);
        enterProcessScreen(newProcess);
    }
}

// 3. Method to open Nvidia SMI processes
void Commands::nvidsmiCommand() {

    //nvidiaSMIView(listAllProcess());
}

// 4. Scheduler Start Command
void Commands::schedulerTestCommand() {
    std::cout << "Scheduler-start: Process generation started." << std::endl;

    if (!scheduler) {
        std::cout << "Scheduler is not initialized. Please run 'initialize' first." << std::endl;
        return;
    }
    scheduler->start();
}

// 5. Scheduler Stop Command
void Commands::schedulerStopCommand() {
    scheduler->stop();
}

// Process reporting and display
void Commands::writeProcessReport(std::ostream& os) {
    os << scheduler->getMetrics();

    /*os << "\nWaiting Queue:\n";
    for (const auto& process : scheduler->getWaitingProcesses()) {
        os << process.processName << "\tProcess created at: " << process.timeStamp
            << "\tCore: N/A\t" << process.currentLine << " / " << process.totalLine << "\n";
    }*/

    /*os << "\nIn Memory:\n";
    for (const auto& process : scheduler->getWaitingMemory()) {
        os << process.processName << "\tProcess created at: " << process.timeStamp
            << "\tCore: N/A\t" << process.currentLine << " / " << process.totalLine << "\n";
    }*/

    os << "\nRunning Processes:\n";
    for (const auto& process : scheduler->getRunningProcesses()) {
        os << process.processName << "\tProcess created at: " << process.arrivalTime
            << "\tCore: " << process.assignedCore << "\t" << process.currentLine << " / " << process.totalLine << "\n";
    }

    os << "\nFinished processes:\n";
    for (const auto& entry : scheduler->getFinishedProcesses()) {
        const auto& process = entry.first;
        int coreId = entry.second;
        os << process.processName << "\tProcess created at: " << process.arrivalTime
            << "\tFinished\tCore: " << coreId
            << "\t" << process.totalLine << " / " << process.totalLine << "\n";
    }
}

void Commands::reportUtilCommand() {
    std::ofstream logFile("csopesy-log.txt");
    if (logFile.is_open()) {
        writeProcessReport(logFile);
        logFile.close();
        std::cout << "Report saved to csopesy-log.txt" << std::endl;
    }
    else {
        std::cerr << "Error: Could not open csopesy-log.txt for writing." << std::endl;
    }
}

void Commands::exitCommand() {
    (getCurrent() == "menuView") ? exit(0) : menuView();
}
