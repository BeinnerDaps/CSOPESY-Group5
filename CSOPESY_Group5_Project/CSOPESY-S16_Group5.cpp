// MAIN

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib> // For clearing functionality

#include "Data.h"
#include "Screen.h"
#include "Commands.h"
Data data;
Screen screen;
Commands commands;

int main() {
    std::string command;

    // Main loop to continuously take in commands, until exit.
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        commands.processCommand(command);
    }

    return 0;
}

