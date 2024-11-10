#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream> 
#include <condition_variable>

#include "ProcessInfo.h"
#include "Config.h"
#include "Data.h"

class Scheduler {
private:
    Config config;
    Data data;
    std::vector<MemoryFrame> memoryPool;
    std::deque<ProcessInfo> processQueue;
    std::deque<ProcessInfo> memoryQueue;
    std::vector<std::pair<ProcessInfo, int>> finishedProcesses;
    std::vector<ProcessInfo> runningProcesses;
    std::vector<std::thread> coreThreads;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool running;
    void coreFunction(int coreId);
    std::vector<ProcessInfo> waitingProcesses;

    bool allocMemory(ProcessInfo& process);
    void deallocMemory(ProcessInfo& process);
    void Memoryreport(int cycle);

    int coresInUse = 0;

public:
    Scheduler(const Config& config);
    ~Scheduler();
    ProcessInfo& getProcess(const std::string& name);
    void start();
    void stop();
    void addProcess(const ProcessInfo& process);
    std::vector<std::pair<ProcessInfo, int>> getFinishedProcesses();
    std::vector<ProcessInfo> getRunningProcesses();
    std::vector<ProcessInfo> getWaitingProcesses();
    std::vector<ProcessInfo> getWaitingMemory();
    std::string getMetrics();
};

#endif
