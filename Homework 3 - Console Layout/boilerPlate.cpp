#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>

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
    std::tm *ltm = std::localtime(&now);
    std::stringstream timestamp;
    timestamp << std::put_time(ltm, "%m/%d/%Y, %I:%M:%S %p");
    return timestamp.str();
}

void printHeader() {
    std::cout << R"(
 _______  _______  _______  _______  _______  _______  __   __ 
|       ||       ||       ||       ||       ||       ||  | |  |
|       ||  _____||   _   ||    _  ||    ___||  _____||  |_|  |
|       || |_____ |  | |  ||   |_| ||   |___ | |_____ |       |
|      _||_____  ||  |_|  ||    ___||    ___||_____  ||_     _|
|     |_  _____| ||       ||   |    |   |___  _____| |  |   |  
|_______||_______||_______||___|    |_______||_______|  |___| 
    )" << std::endl;
}

void clearScreen(bool shouldPrintHeader = true) {
    std::system("cls");
    if (shouldPrintHeader) {
        printHeader();
    }
}



void showCommands() {
    std::cout << "Available commands:\n"
              << "  marquee      - Does something for marquee\n"
              << "  screen       - Starts a new window within the screen\n"
              << "               - -s <name>: It will start a new screen given a name\n"
              << "               - -r <name>: Will reattach a previous screen\n"
              << "  process-smi  - Does something for process-smi\n"
              << "  nvidia-smi   - Does something for nvidia-smi\n"
              << "  clear        - Clears the screen\n"
              << "  exit         - Exits the program\n"
              << "  ?            - Shows this list of commands\n";
}

void displayProcessScreen(const ProcessScreen& ps) {
    std::cout << "Process name: " << ps.processName << std::endl;
    std::cout << "Current line of instruction: " << ps.currentLine << "/" << ps.totalLine << std::endl;
    std::cout << "Timestamp: " << ps.timestamp << std::endl;
}

bool inScreen = false; // To keep track of whether the user is in a screen or not

void handleScreenCommand(const std::string& command) {
    std::string subCommand, name;
    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;  // Extract the subCommand and process name

    if (subCommand == "-s" || subCommand == "-r") {
        auto it = processScreens.find(name);
        if (subCommand == "-s") {
            if (it == processScreens.end()) {
                // Create new process screen
                ProcessScreen ps;
                ps.processName = name;
                ps.currentLine = 0;
                ps.totalLine = 100; // Placeholder value
                ps.timestamp = getCurrentTimestamp();
                processScreens[name] = ps;
                clearScreen(false);
                std::cout << "Created new screen for process: " << name << std::endl;
                displayProcessScreen(ps);
                inScreen = true; 
            } else {
                // Screen with this name already exists
                std::cout << "A screen with the name '" << name << "' already exists. Please choose a different name." << std::endl;
            }
        } else if (subCommand == "-r") {
            if (it != processScreens.end()) {
                clearScreen(false);
                std::cout << "Screen for process: " << name << std::endl;
                displayProcessScreen(it->second);
                inScreen = true; 
            } else {
                std::cout << "No screen found for process: " << name << std::endl;
            }
        }
    } else {
        std::cout << "Invalid screen command format." << std::endl;
    }
}



int main() {
    std::string command;
    printHeader();

  
do {
    std::cout << "Enter command: ";
    std::getline(std::cin, command);

    if (command.empty()) {
        continue;
    } else if (inScreen && command != "exit") {
        std::cout << "Unrecognized command." << std::endl;
    } else if (command == "marquee") {
        std::cout << "Marquee command recognized. Doing something." << std::endl;
    } else if (command == "screen" || command.rfind("screen ", 0) == 0) {
        handleScreenCommand(command);
    } else if (command == "process-smi") {
        std::cout << "Process-smi command recognized. Doing something." << std::endl;
    } else if (command == "nvidia-smi") {
        std::cout << "Nvidia-smi command recognized. Doing something." << std::endl;
    } else if (command == "clear") {
        clearScreen();
    } else if (command == "exit") {
        if (inScreen) {
            inScreen = false; 
            clearScreen();
        } else {
            std::cout << "Exit command recognized. Exiting." << std::endl;
            break;
        }
    } else if (command == "?") {
        showCommands();
    } else {
        std::cout << "Unrecognized command." << std::endl;
    }

} while (true);



    return 0;
}
