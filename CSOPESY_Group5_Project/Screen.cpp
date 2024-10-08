// VIEW - SCREEN

#include "Screen.h"


// Method to update screen
void Screen::updateScreen(const string& newScreen) {
    if (currentScreen != newScreen) {
        previousScreen = currentScreen;
        currentScreen = newScreen;
    } 
    clearScreen();
}


// Method to display main menu screen
void Screen::menuView() {
    updateScreen("menuView");

    cout << banner;
    cout << commandList;
    cout << prompt;
}


// Method to display marquee screen
void Screen::marqueeView() {
    updateScreen("marqueeView");
}


// Method to display -r/-s commands screen
void Screen::rsScreenView(const Data::ProcessInfo& process) {
    updateScreen("rsScreenView");

    cout << "Process Name: " << process.processName << endl;
    cout << "Current Line: " << process.currentLine << endl;
    cout << "Total Lines: " << process.totalLine << endl;
    cout << "Timestamp: " << process.timeStamp << endl;
}


// Method to display -ls command screen
void Screen::lsScreenView(const std::vector<Data::ProcessInfo>& processList) {
    updateScreen("lsScreenView");

    if (!processList.size()) { cout << "Nothing here but us chickens" << endl; }

    for (const auto& process : processList) { 
        cout << "Process Name: " << process.processName << endl;
        cout << "Current Line: " << process.currentLine << endl;
        cout << "Total Lines: " << process.totalLine << endl;
        cout << "Timestamp: " << process.timeStamp << endl;
        cout << "==============================================" << endl;
    }
}


// Method to display Nvidia SMI Interface
void Screen::nvidiaSMIView(const vector<Data::ProcessInfo>& processes) {
    updateScreen("nvidiaSMIview");
    stringstream ss;

    ss << smiheader;
    ss << smitable;

    for (const auto& process : processes) {
        string temp = textCuttOff(process.processName, 36);
        ss << "|    0   N/A   N/A      ";
        ss.width(6); ss << left << process.pid << "  ";
        ss.width(5); ss << left << process.type << "   ";
        ss << temp;
        if (temp.length() < 47) { ss.width(37 - temp.length()); ss << " ";}
        else { ss << "  ";}
        ss.width(1); ss << left << process.gpuMemoryUsage;
        ss.width(6); ss << " ";
        ss << "|" << endl;
    }

    ss << "+-------------------------------------------------------------------------------------+" << endl;

    cout << ss.str() << endl;
}


// Returns string trimmed to fit dimensions
string Screen::textCuttOff(const string& pName, const size_t maxlen) {
    return (pName.length() > maxlen) ? "..." + pName.substr(pName.length() - (maxlen - 3)) : pName;
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