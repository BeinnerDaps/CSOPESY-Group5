#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

struct ProcessScreen {
    std::string processName;
    int currentLine;
    int totalLine;
    std::string timestamp;
};

std::map<std::string, ProcessScreen> processScreens;

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm ltm{};
    localtime_s(&ltm, &now);
    std::stringstream timestamp;
    timestamp << std::put_time(&ltm, "%m/%d/%Y, %I:%M:%S %p");
    return timestamp.str();
}

void printHeader() {
    std::cout << R"(
    *************************************************************************************
    *    _______    _______     ________  _________   __________   _______  __      __  *
    *   //======\  //======\   //======\\ \=======\\  \=========  //======\  %\    //   *
    *  //*         []          11      11  [[      ||  11         []          %\  //    *
    *  []*         \\_______   [[      11  [[      ||  11######   \\_______    %\//     *
    *  []*          ^######\\  [[      11  11######7   11          ^######\\    11      *
    *  \&\                 //  11      11  #1          11_______          //    #1      *
    *   \&#####%7  \%######7   \&######7/  ##         /#########  \%######7     ##      *
    *___________________________________________________________________________________*
    *************************************************************************************
    Welcome to the Serial OS!
    By: CSOPESY_S16 - Group 5

    Type 'exit' to terminate, 'clear' to clear the terminal
    )" << std::endl;
}

void clearScreen(bool shouldPrintHeader = true) {
    std::system("cls");
    if (shouldPrintHeader) {
        printHeader();
    }
}

void showCommands() {
    std::cout << R"(
    Available Commands:
      screen            - Starts a new window within the screen
                        - -s <name>: It will start a new screen given a name
                        - -r <name>: Will reattach a previous screen
      scheduler-test    - Does something for scheduler-test
      scheduler-stop    - Does something for scheduler-stop
      report-util       - Does something for report-util
      clear             - Does something for scheduler-test
      exit              - exit 
    )" << std::endl;
}

void displayProcessScreen(const ProcessScreen& ps) {
    std::cout << "Process name: " << ps.processName << std::endl;
    std::cout << "Current line of instruction: " << ps.currentLine << "/" << ps.totalLine << std::endl;
    std::cout << "Timestamp: " << ps.timestamp << std::endl;
}

bool programRunning = true;
bool insideScreen = false; // Track if currently in a screen

std::string handleScreenCommand(const std::string& command) {
    std::string subCommand, name;
    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;

    if (name.empty()) {
        std::cout << "Screen name cannot be empty." << std::endl;
        return "ScreenError";
    }

    if (subCommand == "-s" || subCommand == "-r") {
        auto it = processScreens.find(name);
        if (subCommand == "-s") {
            if (it == processScreens.end()) {
                ProcessScreen ps{ name, 0, 100, getCurrentTimestamp() };
                processScreens[name] = ps;
                clearScreen(false);
                std::cout << "Created new screen for process: " << name << std::endl;
                return name;
            }
            else {
                std::cout << "A screen with the name '" << name << "' already exists. Please choose a different name." << std::endl;
                return "ScreenError";
            }
        }
        else if (subCommand == "-r") {
            if (it != processScreens.end()) {
                clearScreen(false);
                std::cout << "Screen for process: " << name << std::endl;
                return name;
            }
            else {
                std::cout << "No screen found for process: " << name << std::endl;
                return "ScreenError";
            }
        }
    }
    else {
        std::cout << "Invalid screen command format." << std::endl;
        return "ScreenError";
    }
}

void screenLoop(const std::string& name, bool isNested = false) {
    std::string command;
    bool active = true;
    auto& screen = processScreens[name];
    displayProcessScreen(screen);

    while (active && programRunning) {
        std::cout << "Screen " << name << " - Enter command ('exit' to return to main menu): ";
        std::getline(std::cin, command);
        if (command == "exit") {
            active = false;
            insideScreen = false;
            clearScreen();
            if (!isNested) {
                showCommands();
            }
        }
        else if (command == "screen" || command.rfind("screen ", 0) == 0) {
            std::string result = handleScreenCommand(command);
            if (result != "ScreenError") {
                insideScreen = true;
                // nested screens are screens opened within a prior screen
                screenLoop(result, true);  // Pass true to indicate nested screen
                active = false;  // Exit the current screen loop when a nested screen opens
            }
        }
        else {
            std::cout << "Processing command: " << command << std::endl;
            screen.timestamp = getCurrentTimestamp();
            displayProcessScreen(screen);
        }
    }
}

int main() {
    printHeader();
    showCommands();

    std::string command;
    while (programRunning) {
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        if (command.empty()) {
            continue;
        }
        else if (command == "marquee") {
            std::cout << "Marquee command recognized. Doing something." << std::endl;
        }
        else if (command == "screen" || command.rfind("screen ", 0) == 0) {
            std::string result = handleScreenCommand(command);
            if (result != "ScreenError") {
                insideScreen = true;
                screenLoop(result);  // Switch control to the main loop of the screen (this is our semi-spinlock)
            }
        }
        else if (command == "process-smi") {
            std::cout << "Process-smi command recognized. Doing something." << std::endl;
        }
        else if (command == "nvidia-smi") {
            std::cout << "Nvidia-smi command recognized. Doing something." << std::endl;
        }
        else if (command == "clear") {
            clearScreen();
            showCommands();  // Ensure the commands are reprinted after clear
        }
        else if (command == "exit") {
            std::cout << "Exit command recognized. Exiting program." << std::endl;
            programRunning = false;
        }
        else if (command == "?") {
            showCommands();
        }
        else {
            std::cout << "Unrecognized command." << std::endl;
        }
    }

    return 0;
}
