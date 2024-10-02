// CONTROLLER

#include "Commands.h"
#include "Data.h"
#include "Screen.h"

Data data;
Screen screen;

// Method for processing commands
void Commands::processCommand(const std::string& input) {
    const std::unordered_map<std::string, std::function<void(const std::string&)>> CommandActions = {
        { "screen", [this](const std::string& input) { screenCommand(input); } },
        { "scheduler-test", [this](const std::string&) { schedulerTestCommand(); } },
        { "scheduler-stop", [this](const std::string&) { schedulerStopCommand(); } },
        { "report-util", [this](const std::string&) {  reportUtilCommand(); } },
        { "clear", [this](const std::string&) {  screen.clearScreen() }},
        { "exit", [this](const std::string&) {  exitCommand(); } },
    };

    std::istringstream iss(input);
    std::string command;
    iss >> command;

    auto it = CommandActions.find(command);

    if (it == CommandActions.end()) {
        std::cout << "ERROR: Invalid Command" << std::endl;
        return;
    }

    it->second(input);
}


// Method to process screen commands
void Commands::screenCommand(const std::string& command) {
    std::string subCommand, name;
    std::vector<std::string> subCommands = { "-r", "-s", "-ls"};

    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;

    if (subCommand != "-ls" && name.empty()) {
        std::cout << "ERROR: Process Not Specified" << std::endl;
        return;
    } 

    auto it = std::find(subCommands.begin(), subCommands.end(), subCommand);
    int found = (it != subCommands.end()) ? std::distance(subCommands.begin(), it) : -1;

    auto rs = [&](std::optional<Data::ProcessInfo> process, const std::string& error) {
        (process) ? screen.rsScreenView(*process) : (void)(std::cout << error << std::endl);
    };

    switch (found) {
        case 0: rs(data.getProcess(name), "ERROR: Process '" + name + "' not found."); break;
        case 1: rs(data.createProcess(name), "ERROR: Process '" + name + "' already exists."); break;
        case 2: screen.lsScreenView(data.listAllProcess());  break;
        default: std::cout << "ERROR: Invalid Subcommand"; break;
    }
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

void Commands::exitCommand() {
    (screen.getCurrent() == "menuView") ? exit(0) : screen.menuView();
}