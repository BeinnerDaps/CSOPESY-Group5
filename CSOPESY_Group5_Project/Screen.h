#ifndef SCREEN_H
#define SCREEN_H

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "ProcessInfo.h"

class Screen {

private:

    const std::string banner = R"(
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
    )";

    const std::string commandList = R"(
    Available Commands:

        marquee         - Makes a marquee program  (optional: <refresh rate> <poll rate>)
        screen          - Starts a new window within the screen
                        - -s <name>: start a new screen given a name
                        - -r <name>: open a currently existing screen
                        - -ls : lists all screen processes
        nvidia-smi      - Display Nvidia processes
        scheduler-test  - Does something for scheduler-test
        scheduler-stop  - Does something for scheduler-stop
        report-util     - Does something for report-util
        clear           - clears the screen 
        exit            - exits program
)";

    const std::string prompt = R"(
Enter a command : 
)";


    const std::string smiheader = R"(
+-----------------------------------------------------------------------------------------+ 
| NVIDIA-SMI 551.86                 Driver Version: 551.86         CUDA Version: 12.4     |
+-----------------------------------------+------------------------+----------------------+
| GPU  Name                     TCC/WDDM  | Bus-Id          Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |
|                                         |                        |               MIG M. |
|=========================================+========================+======================|
|   0  NVIDIA GeForce GTX 1080      WDDM  |   00000000:26:00.0  On |                  N/A |
| 28%   37C    P8             11W /  180W |     701MiB /   8192MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+
)";

    const std::string smitable = R"(
+-----------------------------------------------------------------------------------------+
| Processes:                                                                              |
|  GPU   GI   CI        PID   Type   Process name                              GPU Memory |
|        ID   ID                                                               Usage      |
|=========================================================================================|
)";


public:
    Screen() {}

    std::vector<std::function<void()>> screen;

    std::string smioutput;
    std::string currentScreen = "menuView";
    std::string previousScreen = "";

    std::string getCurrent() const { return currentScreen; }

    // Method to update screen
    void updateScreen(const std::string& newScreen);

    // Method to display main menu screen
    void menuView();

    // Method to display marquee screen
    void marqueeView();

    //// Method to display Nvidia SMI Interface
    //void nvidiaSMIView(const vector<ProcessInfo>& processes);

    //// Returns string trimmed to fit dimensions
    //string textCuttOff(const string& pName, const size_t width);

   
    void enterProcessScreen(const ProcessInfo& process);

    void displayProcess(const ProcessInfo& process);

    // Method to clear the screen
    void clearScreen() const;

public:
    virtual ~Screen() = default;
};

#endif
