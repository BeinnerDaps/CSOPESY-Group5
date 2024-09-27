// VIEW

#include <iostream>
#include "Screen.h"

std::string banner = R"(
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
    )";

std::string commandList = R"(
    Available Commands:
      screen
      scheduler-test
      scheduler-stop
      report-util
      clear
      exit
    )";

std::string prompt = R"(Enter a command : )";


void Screen::updateScreen(const std::string& newScreen) {
    if (currentScreen != newScreen) {
        previousScreen = currentScreen;
        currentScreen = newScreen;
    }
}

void Screen::menuView() {
    std::cout << banner;
    std::cout << commandList;
    std::cout << prompt;
}

void Screen::lsScreenView(const ProcessInfo& process) {
    std::cout << "Process Name: " << process.processName << std::endl;
    std::cout << "Current Line: " << process.currentLine << std::endl;
    std::cout << "Total Lines: " << process.totalLine << std::endl;
    std::cout << "Timestamp: " << process.timeStamp << std::endl;
}

void Screen::clearScreen() const {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void Screen::screenLoop(const std::string& name, bool isNested = false) {
    std::string command;
    //bool active = true;
    //auto& screen = processScreens[name];
    //displayProcessScreen(screen);

    //while (active && programRunning) {
    //    std::cout << "Screen " << name << " - Enter command ('exit' to return to main menu): ";
    //    std::getline(std::cin, command);
    //    if (command == "exit") {
    //        active = false;
    //        insideScreen = false;
    //        clearScreen();
    //        if (!isNested) {
    //            showCommands();
    //        }
    //    }
    //    else if (command == "screen" || command.rfind("screen ", 0) == 0) {
    //        std::string result = handleScreenCommand(command);
    //        if (result != "ScreenError") {
    //            insideScreen = true;
    //            // nested screens are screens opened within a prior screen
    //            screenLoop(result, true);  // Pass true to indicate nested screen
    //            active = false;  // Exit the current screen loop when a nested screen opens
    //        }
    //    }
    //    else {
    //        std::cout << "Processing command: " << command << std::endl;
    //        screen.timestamp = getCurrentTimestamp();
    //        displayProcessScreen(screen);
    //    }
    //}
}