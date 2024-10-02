// VIEW

#include "Screen.h"
#include "Data.h"

// Method to update screen
void Screen::updateScreen(const std::string& newScreen) {
    if (currentScreen != newScreen) {
        previousScreen = currentScreen;
        currentScreen = newScreen;
    } 
    clearScreen();
}


// Method to display main menu screen
void Screen::menuView() {
    updateScreen("menuView");
    std::cout << banner;
    std::cout << commandList;
    std::cout << prompt;
}


// Method to display -r/-s commands screen
void Screen::rsScreenView(const Data::ProcessInfo& process) {
    updateScreen("rsScreenView");
    std::cout << "Process Name: " << process.processName << std::endl;
    std::cout << "Current Line: " << process.currentLine << std::endl;
    std::cout << "Total Lines: " << process.totalLine << std::endl;
    std::cout << "Timestamp: " << process.timeStamp << std::endl;
}


// Method to display -ls command screen
void Screen::lsScreenView(const std::vector<Data::ProcessInfo>& processList) {
    updateScreen("lsScreenView");
    if (!processList.size()) { std::cout << "Nothing here but us chickens" << std::endl; }
    for (const auto& process : processList) { 
        std::cout << "Process Name: " << process.processName << std::endl;
        std::cout << "Current Line: " << process.currentLine << std::endl;
        std::cout << "Total Lines: " << process.totalLine << std::endl;
        std::cout << "Timestamp: " << process.timeStamp << std::endl;
        std::cout << "==============================================" << std::endl;
    }
}


// Method to clear the screen
void Screen::clearScreen() const {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

//void Screen::screenLoop(const std::string& name, bool isNested = false) {
//    std::string command;
//    bool active = true;
//    auto& screen = processScreens[name];
//    displayProcessScreen(screen);
//
//    while (active && programRunning) {
//        std::cout << "Screen " << name << " - Enter command ('exit' to return to main menu): ";
//        std::getline(std::cin, command);
//        if (command == "exit") {
//            active = false;
//            insideScreen = false;
//            clearScreen();
//            if (!isNested) {
//                showCommands();
//            }
//        }
//        else if (command == "screen" || command.rfind("screen ", 0) == 0) {
//            std::string result = handleScreenCommand(command);
//            if (result != "ScreenError") {
//                insideScreen = true;
//                // nested screens are screens opened within a prior screen
//                screenLoop(result, true);  // Pass true to indicate nested screen
//                active = false;  // Exit the current screen loop when a nested screen opens
//            }
//        }
//        else {
//            std::cout << "Processing command: " << command << std::endl;
//            screen.timestamp = getCurrentTimestamp();
//            displayProcessScreen(screen);
//        }
//    }
//}