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
    string input;

    // Main loop to continuously take in commands, until exit.
    while (true) {
        cout << "> ";
        getline(cin, input);
        commands.processCommand(input);
    }

    return 0;
}

