#include "Screen.h"

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

// Method to display marquee screen
void Screen::marqueeView() {
    updateScreen("marqueeView");
}

// Function to display process details
void Screen::displayProcess(const ProcessInfo& process) {
    std::cout << "Displaying Process Details:" << std::endl;
    std::cout << "Process Name: " << process.processName << std::endl;
    std::cout << "ID: " << process.processID << std::endl;
    std::cout << "Current Line: " << process.currentLine << std::endl;
    std::cout << "Total Lines: " << process.totalLine << std::endl;
    std::cout << "Timestamp: " << process.arrivalTime << std::endl;
    std::cout << "Status: " << (process.isFinished ? "Finished" : "Not Finished") << std::endl;
}

void Screen::enterProcessScreen(const ProcessInfo& process) {
    updateScreen("rsScreenView");

    std::cout << "Process: " << process.processName << "\nID: " << process.processID
        << "\nTotal Lines: " << process.totalLine << std::endl;

    while (true) {
        if (process.isFinished) {
            std::cout << "Process has finished.\n";
            break;
        }

        std::string command;
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "process-smi") {
            displayProcess(process);
        } else if (command == "exit") {
            std::cout << "Exiting process screen...\n";
            menuView();
            break;
        } else {
            std::cout << "Invalid command. Available commands: 'process-smi', 'exit'.\n";
        }
    }
}


//void Screen::processSMIView() {
//    updateScreen("processSMIview");
//
//    "+-----------
//}

//void Screen::nvidiaSMIView(const vector<Data::ProcessInfo>& processes) {
//    updateScreen("nvidiaSMIview");
//    stringstream ss;
//
//    ss << smiheader;
//    ss << smitable;
//
//    for (const auto& process : processes) {
//
//        ss << right;
//        ss << "|";
//        ss.width(5); ss << "0";
//        ss.width(6); ss << "N/A";
//        ss.width(5); ss << "N/A";
//
//        ss.width(log10(process.pid) + 7); ss << process.pid;
//        ss.width(7); ss << "C+G";
//
//
//        string text = textCuttOff(process.processName, 38);
//        ss.width(text.length() + 3); ss << text;
//
//
//        ss.width((38 - text.length()) + 9); ss << "N/A";
//        ss.width(7); ss << "|" << endl;
//    }
//
//    ss << "+-----------------------------------------------------------------------------------------+" << endl;
//
//    cout << ss.str() << endl;
//}


//// Returns string trimmed to fit dimensions
//string Screen::textCuttOff(const string& pName, const size_t maxlen) {
//    return (pName.length() > maxlen) ? "..." + pName.substr(pName.length() - (maxlen - 3)) : pName;
//}

void Screen::clearScreen() const {
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
}