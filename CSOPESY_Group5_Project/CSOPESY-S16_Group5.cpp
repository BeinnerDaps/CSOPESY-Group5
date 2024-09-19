#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib> // For clearing functionality


void displayHeader() {
    std::cout << "*************************************************************************************" << std::endl;
    std::cout << "*    _______    _______     ________  _________   __________   _______  __      __  *" << std::endl;
    std::cout << "*   //======\\  //======\\   //======\\\\ \\=======\\\\  \\=========  //======\\  %\\    //   *" << std::endl;
    std::cout << "*  //*         []          11      11  [[      ||  11         []          %\\  //    *" << std::endl;
    std::cout << "*  []*         \\\\_______   [[      11  [[      ||  11######   \\\\_______    %\\//     *" << std::endl;
    std::cout << "*  []*          ^######\\\\  [[      11  11######7   11          ^######\\\\    11      *" << std::endl;
    std::cout << "*  \\&\\                 //  11      11  #1          11_______          //    #1      *" << std::endl;
    std::cout << "*   \\&#####%7  \\%######7   \\&######7/  ##         /#########  \\%######7     ##      *" << std::endl;
    std::cout << "*___________________________________________________________________________________*" << std::endl;
    std::cout << "*************************************************************************************" << std::endl;
    std::cout << "Welcome to the Serial OS!" << std::endl;
    std::cout << "By: CSOPESY_S16 - Group 5" << std::endl;
    std::cout << "\nType 'exit' to terminate, 'clear' to clear the terminal\n" << std::endl;

    std::cout << "Available Commands:" << std::endl;
    // std::cout << "  initialize" << std::endl;
    std::cout << "  screen" << std::endl;
    std::cout << "  scheduler-test" << std::endl;
    std::cout << "  scheduler-stop" << std::endl;
    std::cout << "  report-util" << std::endl;
    std::cout << "  clear" << std::endl;
    std::cout << "  exit" << std::endl;

    std::cout << "\nEnter a command: " << std::endl;
}

void clearScreen() {
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
}

void initializeCommand() {
    std::cout << "initialize command recognized. Doing something." << std::endl;
}

void screenCommand(const std::string& command) { 
    std::string subCommand, name;

    std::vector<std::string> subCommands = { "-r", "-s" };
    bool found = false;
     
    for (const auto& option : subCommands) {
        if (command.find(option) != std::string::npos) {
            found = true; break;
        }
    }

    if (!found) {
        std::cout << "ERROR: Invalid Subcommand" << std::endl;
        return;
    }

    std::istringstream iss(command);
    iss >> subCommand >> subCommand >> name;
    if (name.empty()) {
        std::cout << "ERROR: Process Not Specified" << std::endl;
        return;
    }

    if (subCommand == "-r") {
        
    }

    if (subCommand == "-s") {
        
    }
}

void schedulerTestCommand() {
    std::cout << "scheduler-test command recognized. Doing something." << std::endl;
}

void schedulerStopCommand() {
    std::cout << "scheduler-stop command recognized. Doing something." << std::endl;
}

void reportUtilCommand() {
    std::cout << "report-util command recognized. Doing something." << std::endl;
}

void processCommand(const std::string& command) {
    if (command.find("screen") != std::string::npos) {
        screenCommand(command);
    } else if (command == "scheduler-test") {
        schedulerTestCommand();
    } else if (command == "scheduler-stop") {
        schedulerStopCommand();
    } else if (command == "report-util") {
        reportUtilCommand();
    } else if (command == "clear") {
        clearScreen(); displayHeader();
    } else if (command == "exit") {
        std::cout << "Terminating Serial OS, Thank you!" << std::endl;
        exit(0);
    } else {
        std::cout << "ERROR: Unrecognized command." << std::endl;
    }
}

int main() {
    std::string command;
    clearScreen();  displayHeader();

    // Main loop to continuously take in commands, until exit.
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        processCommand(command);
    }

    return 0;
}

