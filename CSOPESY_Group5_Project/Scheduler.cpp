#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>

Scheduler::Scheduler(const Config& config) : config(config), running(true) {
    for (int i = 0; i < config.numCpu; ++i) {
        coreThreads.emplace_back(&Scheduler::coreFunction, this, i + 1); // Start core IDs from 1
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
    processQueue.push(process);
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

    // Check waiting queue
    std::queue<ProcessInfo> tempQueue = processQueue;
    while (!tempQueue.empty()) {
        if (tempQueue.front().processName == name) {
            return tempQueue.front();
        }
        tempQueue.pop();
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
    std::vector<ProcessInfo> waitingQueue;
    std::queue<ProcessInfo> tempQueue = processQueue;

    while (!tempQueue.empty()) {
        waitingQueue.push_back(tempQueue.front());
        tempQueue.pop();
    }
    return waitingQueue;
}

void Scheduler::coreFunction(int coreId) {
    while (running) {
        ProcessInfo process(-1, "defaultProcess", 100, "Timestamp");

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return !processQueue.empty() || !running; });

            if (!running && processQueue.empty()) break;

            if (!processQueue.empty()) {
                process = processQueue.front();
                processQueue.pop();
                process.assignedCore = coreId;
                runningProcesses.push_back(process);
            }
            else {
                continue;
            }
        }

        // Process execution loop
        for (int i = 0; i < process.totalLine; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(config.delaysPerExec));
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                process.currentLine++;

                auto it = std::find_if(runningProcesses.begin(), runningProcesses.end(),
                    [&](const ProcessInfo& p) { return p.processName == process.processName; });
                if (it != runningProcesses.end()) {
                    it->currentLine = process.currentLine;
                }

                if (process.currentLine >= process.totalLine) {
                    process.isFinished = true; // Mark as finished
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            // Move finished process to finishedProcesses
            finishedProcesses.push_back({ process, coreId });
            runningProcesses.erase(std::remove_if(runningProcesses.begin(), runningProcesses.end(),
                [&](const ProcessInfo& p) { return p.processName == process.processName; }),
                runningProcesses.end());
        }
        cv.notify_one();
    }
}
