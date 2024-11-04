#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>

Scheduler::Scheduler(const Config& config)
    : config(config), running(true), schedulerType(config.scheduler), quantum(config.quantumCycles) {
    for (int i = 0; i < config.numCpu; ++i) {
        coreThreads.emplace_back(&Scheduler::coreFunction, this, i + 1);  // Start core IDs from 1
    }
}

Scheduler::~Scheduler() {
    stop();
    for (auto& thread : coreThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Scheduler::start() {
    running = true;
}

void Scheduler::stop() {
    running = false;
    cv.notify_all();
}

void Scheduler::addProcess(const ProcessInfo& process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push_back(process);
    cv.notify_one();
}

ProcessInfo& Scheduler::getProcess(const std::string& name) {
    std::lock_guard<std::mutex> lock(queueMutex);

    // Check running processes
    for (auto& process : runningProcesses) {
        if (process.processName == name) {
            return process;
        }
    }

    // Check waiting queue (processQueue)
    for (auto& process : processQueue) {
        if (process.processName == name) {
            return process;
        }
    }

    throw std::runtime_error("Process not found: " + name);
}

std::vector<std::pair<ProcessInfo, int>> Scheduler::getFinishedProcesses() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return finishedProcesses;
}

std::vector<ProcessInfo> Scheduler::getRunningProcesses() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return runningProcesses;
}

std::vector<ProcessInfo> Scheduler::getWaitingProcesses() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return std::vector<ProcessInfo>(processQueue.begin(), processQueue.end());
}

void Scheduler::coreFunction(int coreId) {
    while (running) {
        ProcessInfo process(-1, "defaultProcess", 100, "Timestamp");
        bool processAssigned = false;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return !processQueue.empty() || !running; });

            if (!running && processQueue.empty()) break;

            process = processQueue.front();
            processQueue.pop_front();
            process.assignedCore = coreId;
            processAssigned = true;

            runningProcesses.push_back(process);

            ++coresInUse;
        }

        if (!processAssigned) {
            continue;
        }

        bool processCompleted = false;
        int executedCycles = 0;
        int execCycles = (schedulerType == "rr") ? quantum : process.totalLine;
        int delay = (config.delaysPerExec < 1) ? 1 : config.delaysPerExec;

        while (executedCycles < execCycles && process.currentLine < process.totalLine) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            process.currentLine++;
            executedCycles++;

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                auto it = std::find_if(runningProcesses.begin(), runningProcesses.end(),
                    [&](const ProcessInfo& p) { return p.processID == process.processID; });
                if (it != runningProcesses.end()) {
                    it->currentLine = process.currentLine;
                }
            }

            if (process.currentLine >= process.totalLine) {
                processCompleted = true;
                break;
            }
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            runningProcesses.erase(std::remove_if(runningProcesses.begin(), runningProcesses.end(),
                [&](const ProcessInfo& p) { return p.processID == process.processID; }),
                runningProcesses.end());
            --coresInUse;
        }

        if (processCompleted) {
            std::lock_guard<std::mutex> lock(queueMutex);
            finishedProcesses.push_back({ process, coreId });
            // std::cout << "[DEBUG] Process " << process.processName << " completed on core " << coreId << std::endl;
        }
        else if (schedulerType == "rr") {
            std::lock_guard<std::mutex> lock(queueMutex);
            processQueue.push_back(process);
            // std::cout << "[DEBUG] Process " << process.processName << " quantum depleted, re-queuing on core " << coreId << std::endl;
        }

    cv.notify_all();
    std::this_thread::sleep_for(std::chrono::milliseconds(config.batchProcessFreq));
    }
}

std::string Scheduler::getMetrics() {
    // CPU Utilization: percentage of active core time
    double cpuUtilization = (coresInUse * 100.0) / config.numCpu;

    // Cores Used and Cores Available
    int coresAvailable = config.numCpu - coresInUse;

    std::ostringstream os;
    os << "\nCPU Utilization: " << cpuUtilization << "%\n";
    os << "Cores Used: " << coresInUse << "\n";
    os << "Cores Available: " << coresAvailable << "\n\n";
    return os.str();
}


