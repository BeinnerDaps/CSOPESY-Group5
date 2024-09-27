// Screen.h
#ifndef SCREEN_H
#define SCREEN_H

#include <string>

#include "Data.h"

class Screen {

protected:
    std::vector<std::function<void()>> screen;

    std::string currentScreen;
    std::string previousScreen;

    void menuView();

    void lsScreenView(const ProcessInfo& process);

    // Clear the Screen
    void clearScreen() const;

    // Screen Loop
    void screenLoop(const std::string& name, bool isNested);

    // Update Screen
    void updateScreen(const std::string& newScreen);

public:


};

#endif