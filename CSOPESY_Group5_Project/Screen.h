// Screen.h
#ifndef SCREEN_H
#define SCREEN_H

#include "Data.h"

#include <string>
#include <vector>
#include <functional>


class Screen: public Data {

public:

    Screen(){}

    std::vector<std::function<void()>> screen;

    std::string currentScreen = "menuView";
    std::string previousScreen = "";

    // Method to choose screen to display
    //void displayScreen(const std::string& screenName, const std::string& argument);

    // Method to display main menu screen
    void menuView(const std::string& banner, const std::string& commandList, const std::string& prompt);

    // Method to display -r/-s commands screen
    void rsScreenView(ProcessInfo process);

    // Method to display -ls command screen
    void lsScreenView(ProcessInfo process);

    // Method to clear the screen
    void clearScreen() const;

    // Method to regularly update screen with loops
    void screenLoop(const std::string& name, bool isNested);

    // Method to update screen
    void updateScreen(const std::string& newScreen);


};

#endif