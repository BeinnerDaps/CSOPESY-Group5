#ifndef COMMANDS_H
#define COMMANDS_H

#include "Config.h"
#include "Data.h"
#include "Marquee.h"
#include "Scheduler.h"
#include "Screen.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <windows.h> 

class Commands : public Screen, public Data {

private:
    Config config;
    std::mutex queueMutex;
    std::unique_ptr<Scheduler> scheduler;
    std::vector<ProcessInfo> processList;

    void writeProcessReport(std::ostream& os);
    Config parseConfigFile(const std::string& filename);
    


public:
    Commands();

    void marqueeCommand(const std::string& command);
    void nvidsmiCommand();
    void initialize(const std::string& filename);
    void processCommand(const std::string& command);
    void screenCommand(const std::string& command);
    void rSubCommand(const std::string& name);
    void sSubCommand(const std::string& name);
    void schedulerTestCommand();
    void schedulerStopCommand();
    void reportUtilCommand();
    void exitCommand();
};

#endif
