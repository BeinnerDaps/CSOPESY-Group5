#include <iostream>
#include <string>
#include <cstdlib> 

using namespace std;

void printMenuHeader() {


    std::cout << "===== Main Menu =====" << std::endl;
    std::cout << "Available Commands:" << std::endl;
    std::cout << "1. initialize" << std::endl;
    std::cout << "2. screen" << std::endl;
    std::cout << "3. scheduler-test" << std::endl;
    std::cout << "4. scheduler-stop" << std::endl;
    std::cout << "5. report-util" << std::endl;
    std::cout << "6. clear" << std::endl;
    std::cout << "7. exit" << std::endl;
    std::cout << "=====================" << std::endl;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    std::string command;
    bool running = true;

    printMenuHeader();

    while (running) {
        std::cout << "\nEnter command: ";
        std::getline(std::cin, command);

        if (command == "initialize" || command == "1" ) {
            std::cout << "Initialize command recognized. Doing something." << std::endl;
        } 
        else if (command == "screen"|| command == "2" ) {
            std::cout << "Screen command recognized. Doing something." << std::endl;
        } 
        else if (command == "scheduler-test"|| command == "3" ) {
            std::cout << "Scheduler-test command recognized. Doing something." << std::endl;
        } 
        else if (command == "scheduler-stop"|| command == "4" ) {
            std::cout << "Scheduler-stop command recognized. Doing something." << std::endl;
        } 
        else if (command == "report-util"|| command == "5" ) {
            std::cout << "Report-util command recognized. Doing something." << std::endl;
        } 
        else if (command == "clear"|| command == "6" ) {
            clearScreen();  // Clears the screen
            printMenuHeader();  // Reprints the menu header
        } 
        else if (command == "exit"|| command == "7" ) {
            std::cout << "Exiting the application." << std::endl;
            running = false;  // Exit the application
        } 
        else {
            std::cout << "Unknown command. Please try again." << std::endl;
        }
    }

    return 0;
}

