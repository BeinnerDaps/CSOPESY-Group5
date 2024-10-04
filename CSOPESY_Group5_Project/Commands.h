// Commands.h
#ifndef COMMANDS_H
#define COMMANDS_H

#include "Data.h"
#include "Screen.h"
#include "Marquee.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
using namespace std;

class Commands {

private:
    Data data;
    Screen screen;
 
public:

    Commands() {}

    //  Method to execute a marquee process
    void marqueeCommand(const string& command);

    // Method for processing commands
    void processCommand(const string& command);

    // Method to process screen commands
    void screenCommand(const string& command);

    // Method to show Nvidia-smi screen
    void nvidsmiCommand();
    
    //  Method to process scheduler test commands
    void schedulerTestCommand();

    //  Method to process scheduler stop commands
    void schedulerStopCommand();

    //  Method to process report util commands
    void reportUtilCommand();

    //  Method to exit out screen
    void exitCommand();
};

#endif
