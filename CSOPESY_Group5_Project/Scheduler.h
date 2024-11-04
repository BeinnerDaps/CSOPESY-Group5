#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Data.h"
#include "Screen.h"

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <thread>
#include <vector>
using namespace std;

class Scheduler: public Data {

private:
    queue<ProcessInfo> processQueue;
    vector<pair<ProcessInfo, int>> finishedProcesses;
    vector<thread> coreThreads;
    mutex queueMutex;
    condition_variable cv;
    bool running;
    int cores;

    void coreFunction(int coreId);

public:

    Scheduler(int cores) : cores(cores) {};
    ~Scheduler();

    void addProcess(const ProcessInfo& process);
    void start();
    void stop();
    vector<pair<ProcessInfo, int>> getFinishedProcesses();
    vector<ProcessInfo> getRunningProcesses();
};

#endif
