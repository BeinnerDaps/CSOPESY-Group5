// Commands.h
#ifndef COMMANDS_H
#define COMMANDS_H

#include "Data.h"

#include <string>


class Commands: protected Data {

public:

    Commands(){}

    // Method for processing commands
    void processCommand(const std::string& command);

    // Method to process screen commands
    void screenCommand(const std::string& command);
    
    //  Method to process scheduler test commands
    void schedulerTestCommand();

    //  Method to process scheduler stop commands
    void schedulerStopCommand();

    //  Method to process report util commands
    void reportUtilCommand();
};

#endif
