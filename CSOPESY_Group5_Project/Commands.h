// Commands.h
#ifndef COMMANDS_H
#define COMMANDS_H

#include "Screen.h"
#include "Data.h"
#include <string>

class Commands: protected Screen, public Data {

public:
    void initialScreen();

    // processCommand - processes command
    void processCommand(const std::string& command);

    // Command for screen
    void screenCommand(const std::string& command);

    // Method for the -r subcommand
    void rSubCommand(const std::string& name);

    // Method for the -s subcommand
    void sSubCommand(const std::string& name);

    void lsSubCommand();

    void schedulerTestCommand();

    void schedulerStopCommand();

    void reportUtilCommand();
};

#endif
