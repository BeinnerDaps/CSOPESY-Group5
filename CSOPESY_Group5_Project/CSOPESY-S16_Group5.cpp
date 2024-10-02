// MAIN

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

#include "Data.h"
#include "Screen.h"
#include "Commands.h"

int main() {
    Screen screen;
    screen.menuView();

    Commands commands;
    string command;
    
    // Main loop to continuously take in commands, until exit.
    while (true) {
        cout << "> ";
        getline(std::cin, command);
        commands.processCommand(command);
    }

    return 0;
}

