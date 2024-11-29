#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <random>
#include <sstream> 
#include <thread>
#include <vector>

#include "ProcessInfo.h"
#include "Config.h"
#include "Data.h"

#include "json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

class Scheduler {

private:

    Config config;
    Data data;

    std::atomic<bool> running, generator;

    std::vector<MemoryFrame> memoryPool;

    std::deque<ProcessInfo> processQueue;
    std::deque<ProcessInfo> memoryQueue;
    std::deque<ProcessInfo> backingStore;

    std::vector<std::pair<ProcessInfo, int>> finishedProcesses;
    std::vector<ProcessInfo> runningProcesses;

    std::vector<std::thread> coreThreads;
    std::thread generatorThread;
    std::thread waitingQueue;

    std::mutex queueMutex;
    std::mutex fileMutex;
    std::mutex memPoolMutex;
    std::condition_variable cv;
    
    void coreFunction(int coreId);
    void processGenerator();

    void checkWaiting();
    bool allocMemory(ProcessInfo& process);
    bool deallocMemory(ProcessInfo& process);
    bool evictOldestProc();
 
    bool logBackingStore(const ProcessInfo& process, const std::string& action);
    bool writeBackingStore(const ProcessInfo& process);
    void readBackingStore();
    void clearJsonFile(const std::string& fileName);

    void Memoryreport(int cycle);

    int processID = 1;
    int coresInUse = 0;
    int procInMem = 0;
    int counter = 1;

public:
    Scheduler(const Config& config);
    ~Scheduler();

    void start();
    void stop();

    void addProcess(ProcessInfo& process);
    ProcessInfo& getProcess(const std::string& name);
    
    int getRandomInt(int floor, int ceiling);
    
    std::vector<std::pair<ProcessInfo, int>> getFinishedProcesses();
    std::vector<ProcessInfo> getRunningProcesses();
    std::vector<ProcessInfo> getWaitingProcesses();
    std::vector<ProcessInfo> getWaitingMemory();
    std::string getMetrics();
};

#endif
