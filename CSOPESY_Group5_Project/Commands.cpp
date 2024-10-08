// CONTROLLER

#include "Commands.h"

Data data;
Screen screen;


// Method for processing commands
void Commands::processCommand(const string& input) {
    const std::unordered_map<string, function<void(const string&)>> CommandActions = {
        { "marquee", [this](const string& input) { marqueeCommand(input); } },
        { "screen", [this](const string& input) { screenCommand(input); } },
        { "nvidia-smi", [this](const string&) { nvidsmiCommand(); } },
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
    iss >> text >> text >> refresh >> poll;

    if (!refresh) { refresh = 50; } // Default refresh rate
    if (!poll) { poll = 50; } // Default poll rate
    if (text.empty()) { text = "threaded"; }

    Marquee marquee(refresh, poll, true);

    if (text == "threaded") { marquee.startThread(); screen.marqueeView(); }
    else if (text == "nonthreaded") { marquee.startNonThread(); screen.marqueeView(); }
    else { cout << "ERROR: Incomplete parameters" << endl; }

    screen.menuView();
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

    auto displayProc = [&](optional<Data::ProcessInfo> process, const string& error) {
        (process) ? screen.rsScreenView(*process) : (void)(cout << error << endl);
    };

    auto it = find(subCommands.begin(), subCommands.end(), subCommand);
    int found = (it != subCommands.end()) ? distance(subCommands.begin(), it) : -1;

    switch (found) {
        case 0: displayProc(data.getProcess(name), "ERROR: Process '" + name + "' not found."); break;
        case 1: displayProc(data.createProcess(name), "ERROR: Process '" + name + "' already exists."); break;
        case 2: screen.lsScreenView(data.listAllProcess());  break;
        default: cout << "ERROR: Invalid Subcommand" << endl; break;   
    }
}


// Method to open Nvidia SMI processes
void Commands::nvidsmiCommand() {

    data.processList.emplace_back("C:\\Users\\user\\Downloads\\File.exe", data.processList.size() + 1, data.processList.size() + 1, data.getTime(), 1368, "C+G", "N/A");
    data.processList.emplace_back("C:\\Users\\user\\DLSU\\CSOPESY_PROJECT.exe", data.processList.size() + 1, data.processList.size() + 1, data.getTime(), 8024, "C+G", "N/A");
    data.processList.emplace_back("C:\\Users\\user\\personal\\Onedrive\\Project1\\Verylongfolderorfilename\\wordfile.docx", data.processList.size() + 1, data.processList.size() + 1, data.getTime(), 2913, "C+G", "N/A");
    data.processList.emplace_back("C:\\Users\\user\\Acads\\Homework\\Video.mp4", data.processList.size() + 1, data.processList.size() + 1, data.getTime(), 1371, "C+G", "N/A");
    data.processList.emplace_back("C:\\Users\\user\\:E", data.processList.size() + 1, data.processList.size() + 1, data.getTime(), 1313, "C+G", "N/A");

    screen.nvidiaSMIView(data.listAllProcess());
}


//  Method to process scheduler test commands
void Commands::schedulerTestCommand() {
    std::cout << "scheduler-test command recognized. Doing something." << std::endl;
}


//  Method to process scheduler stop commands
void Commands::schedulerStopCommand() {
    std::cout << "scheduler-stop command recognized. Doing something." << std::endl;
}


//  Method to process report util commands
void Commands::reportUtilCommand() {
    std::cout << "report-util command recognized. Doing something." << std::endl;
}


//  Method to exit out screen
void Commands::exitCommand() {
    (screen.getCurrent() == "menuView") ? exit(0) : screen.menuView();
}