// CONTROLLER

#include "Commands.h"

Data data;
Screen screen;


// Method for processing commands
void Commands::processCommand(const string& input) {
    const std::unordered_map<string, function<void(const string&)>> CommandActions = {
        { "marquee", [this](const string& input) { marqueeCommand(input); } },
        { "screen", [this](const string& input) { screenCommand(input); } },
        { "scheduler-test", [this](const string&) { schedulerTestCommand(); } },
        { "scheduler-stop", [this](const string&) { schedulerStopCommand(); } },
        { "report-util", [this](const string&) {  reportUtilCommand(); } },
        { "clear", [this](const string&) {  screen.clearScreen(); } },
        { "exit", [this](const string&) {  exitCommand(); } }
    };

    istringstream iss(input);
    string command;
    iss >> command;

    auto it = CommandActions.find(command);

    if (it == CommandActions.end()) {
        cout << "ERROR: Invalid Command" << endl;
        return;
    }

    it->second(input);
}


//  Method to execute a marquee process
void Commands::marqueeCommand(const string& command) {
    bool run = true;

    istringstream iss(command);
    string text;
    int refresh = 0, poll = 0;
    iss >> text >> refresh >> poll;

    if (!refresh) { refresh = 50; } // Default refresh rate
    if (!poll) { poll = 50; } // Default poll rate

    Marquee marquee(refresh, poll, true);

    screen.marqueeView();
    marquee.start();
}

// Method to process screen commands
void Commands::screenCommand(const string& command) {
    string subCommand, name;
    vector<string> subCommands = { "-r", "-s", "-ls"};

    istringstream iss(command);
    iss >> subCommand >> subCommand >> name;

    if (subCommand != "-ls" && name.empty()) {
        cout << "ERROR: Process Not Specified" << endl;
        return;
    } 

    auto rs = [&](optional<Data::ProcessInfo> process, const string& error) {
        (process) ? screen.rsScreenView(*process) : (void)(cout << error << endl);
    };

    auto it = find(subCommands.begin(), subCommands.end(), subCommand);
    int found = (it != subCommands.end()) ? distance(subCommands.begin(), it) : -1;

    switch (found) {
        case 0: rs(data.getProcess(name), "ERROR: Process '" + name + "' not found."); break;
        case 1: rs(data.createProcess(name), "ERROR: Process '" + name + "' already exists."); break;
        case 2: screen.lsScreenView(data.listAllProcess());  break;
        default: cout << "ERROR: Invalid Subcommand" << endl; break;   
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