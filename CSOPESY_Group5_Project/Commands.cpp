// CONTROLLER - COMMANDS

#include "Commands.h"

Data data;
Screen screen;
Scheduler scheduler;

// Method for processing commands
void Commands::processCommand(const string& input) {
    const unordered_map<string, function<void(const string&)>> CommandActions = {
        { "marquee", [this](const string& input) { marqueeCommand(input); } },
        { "screen", [this](const string& input) { screenCommand(input); } },
        { "nvidia-smi", [this](const string&) { nvidsmiCommand(); } },
        { "scheduler-test", [this](const string& input) { schedulerTestCommand(input); } },
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
        case 2: screen.lsScreenView(data.listAllProcess(), scheduler.getFinishedProcesses(), scheduler.getRunningProcesses());  break;
        default: cout << "ERROR: Invalid Subcommand" << endl; break;   
    }
}


// Method to open Nvidia SMI processes
void Commands::nvidsmiCommand() {

    data.processList.emplace_back("C:\\Users\\user\\Downloads\\File.exe", data.getTime(), 1368);
    data.processList.emplace_back("C:\\Users\\user\\DLSU\\CSOPESY_PROJECT.exe", data.getTime(), 8024);
    data.processList.emplace_back("C:\\Users\\user\\personal\\Onedrive\\Project1\\Verylongfolderorfilename\\wordfile.docx", data.getTime(), 2913);
    data.processList.emplace_back("C:\\Users\\user\\Acads\\Homework\\Video.mp4", data.getTime(), 1371);
    data.processList.emplace_back("C:\\Users\\user\\:E", data.getTime(), 1313);

    screen.nvidiaSMIView(data.listAllProcess());
}

//  Method to process scheduler test commands
void Commands::schedulerTestCommand(const string& command) {

    int processnum;
    istringstream iss(command);
    iss >> processnum >> processnum;

    if (!processnum) { processnum = 10; } // Default processes

    cout << "Scheduling" << processnum << "processes on 4 CPU Cores(Check via screen - ls)" << endl;

    // Initialize processes
    for (int i = 1; i <= processnum; ++i) { 
        data.createProcess("process" + to_string(i));
        Sleep(data.getRandomNumber(500, 2000));
    }

    // Add process to scheduler
    for (auto& process : data.processList) { scheduler.addProcess(process); }
}


//  Method to process scheduler stop commands
void Commands::schedulerStopCommand() {
    cout << "scheduler-stop command recognized. Doing something." << endl;
    scheduler.stop();
}


//  Method to process report util commands
void Commands::reportUtilCommand() {
    std::cout << "report-util command recognized. Doing something." << std::endl;
}


//  Method to exit out screen
void Commands::exitCommand() {
    (screen.getCurrent() == "menuView") ? exit(0) : screen.menuView(); schedulerStopCommand();
}