// Screen.h
#ifndef SCREEN_H
#define SCREEN_H

#include "Data.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Screen {

private:

const string banner = R"(
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

const string commandList = R"(
    Available Commands:
        marquee         - Does something for marquee
        screen          - Starts a new window within the screen
                        - -s <name>: start a new screen given a name
                        - -r <name>: open a currently existing screen
        scheduler-test  - Does something for scheduler-test
        scheduler-stop  - Does something for scheduler-stop
        report-util     - Does something for report-util
        clear           - clears the screen 
        exit            - exits program
)";

const string prompt = R"(
Enter a command : 
)";

public:

    Screen(){}

    vector<function<void()>> screen;

    string currentScreen = "menuView";
    string previousScreen = "";

    const string getCurrent() const { return currentScreen; }

    // Method to update screen
    void updateScreen(const string& newScreen);

    // Method to display main menu screen
    void menuView();

    // Method to display marquee screen
    void marqueeView(int x, int y, const string& value);

    // Method to display -r/-s commands screen
    void rsScreenView(const Data::ProcessInfo& process);

    // Method to display -ls command screen
    void lsScreenView(const vector<Data::ProcessInfo>& processList);

    // Method to clear the screen
    void clearScreen() const;

    // Method to regularly update screen with loops
    // void screenLoop(const std::string& name, bool isNested);
};

#endif