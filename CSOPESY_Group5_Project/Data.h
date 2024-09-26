// Data.h
#ifndef DATA_H
#define DATA_H

#include "Screen.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// Struct containing process information
struct ProcessInfo {
    std::string processName;
    int currentLine;
    int totalLine;
    std::string timeStamp;
};

class Data: public Screen {

public:

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

    Type 'exit' to terminate, 'clear' to clear the terminal
    )";

    const std::string commandList = R"(
    Available Commands:
      screen
      scheduler-test
      scheduler-stop
      report-util
      clear
      exit
    )";

    const std::string prompt = R"(
    Enter a command : 
    )";

protected:

    Data(){}

    // List of processes
    std::vector<ProcessInfo> processList;


    // Method to return to menu
    void getMenu();

    // Method to return to menu or close program
    void exitOut();

    // Method to display a specific process
    void getProcess(const std::string& processName);

    // Method to create a new process
    void createProcess(const std::string& processName);

    // Method to list all processes
    void listAllProcess(); 

    // Method to update processes
    //ProcessInfo& updateProcess();

    // Get current Timestamp
    std::string getTimestamp() const;
};

#endif