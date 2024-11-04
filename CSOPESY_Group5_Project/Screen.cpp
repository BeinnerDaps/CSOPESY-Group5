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
    cout << "Timestamp: " << process.arrivalTime << endl;
}


// Method to display -ls command screen
void Screen::lsScreenView(const vector<Data::ProcessInfo>& processList, const vector<pair<Data::ProcessInfo, int>> finishedPs, const vector<Data::ProcessInfo> runningPs) {
    updateScreen("lsScreenView");

    if (!processList.size()) { 
        cout << "Nothing here but us chickens" << endl; 
    } 
    else
    {
        // Display running processes
        cout << "Waiting Queue:" << endl;
        for (const auto& process : processList) {
            auto it = find_if(finishedPs.begin(), finishedPs.end(),
                [&](const pair<Data::ProcessInfo, int>& p) { return p.first.processName == process.processName; });

            if (it == finishedPs.end()) {
                auto runningIt = find_if(runningPs.begin(), runningPs.end(),
                    [&](const Data::ProcessInfo& p) { return p.processName == process.processName; });

                if (runningIt != runningPs.end()) {
                    cout << process.processName << "\tArrival Index: " << process.arrivalTime
                        << "\tCore: N/A\t" << process.currentLine << " / " << process.totalLine << endl;
                }
            }
        }

        // Display finished processes
        cout << "\nFinished processes:" << endl;
        for (const auto& entry : finishedPs) {
            const auto& process = entry.first;
            int coreId = entry.second;
            cout << process.processName << "\tArrival Index: " << process.arrivalTime
                << "\tFinished\tCore: " << coreId
                << "\t" << process.totalLine << " / " << process.totalLine << endl;
        }
    }
}


// Method to display Nvidia SMI Interface
void Screen::nvidiaSMIView(const vector<Data::ProcessInfo>& processes) {
    updateScreen("nvidiaSMIview");
    stringstream ss;

    ss << smiheader;
    ss << smitable;

    for (const auto& process : processes) {
        
        ss << right;
        ss << "|";
        ss.width(5); ss << "0";
        ss.width(6); ss << "N/A";
        ss.width(5); ss << "N/A";

        ss.width(log10(process.pid) + 7); ss << process.pid;
        ss.width(7); ss << "C+G";
        

        string text = textCuttOff(process.processName, 38);
        ss.width(text.length() + 3); ss << text;


        ss.width((38-text.length()) + 9); ss << "N/A";
        ss.width(7); ss << "|" << endl;
    }

    ss << "+-----------------------------------------------------------------------------------------+" << endl;

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