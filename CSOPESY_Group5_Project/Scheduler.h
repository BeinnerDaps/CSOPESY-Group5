#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "ProcessInfo.h"

class Scheduler {
private:
    std::queue<ProcessInfo> processQueue;
    std::vector<ProcessInfo> runningProcesses;
    std::vector<std::pair<ProcessInfo, int>> finishedProcesses;
    std::vector<std::thread> coreThreads;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool running;

    void coreFunction(int coreId);

public:
    Scheduler();
    ~Scheduler();

    void addProcess(const ProcessInfo& process);
    void start();
    void stop();

    std::vector<std::pair<ProcessInfo, int>> getFinishedProcesses();
    std::vector<ProcessInfo> getRunningProcesses();
};

#endif
