#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <thread>

Scheduler::Scheduler() : running(true) {
    // Initialize threads representing the 4 cores
    for (int i = 0; i < 4; ++i) {
        coreThreads.emplace_back(&Scheduler::coreFunction, this, i);
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

void Scheduler::addProcess(const ProcessInfo& process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push(process);
    cv.notify_one();
}

void Scheduler::start() {
    running = true;
}

void Scheduler::stop() {
    running = false;
    cv.notify_all();
}

void Scheduler::coreFunction(int coreId) {
    while (true) {
        ProcessInfo process;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return !processQueue.empty() || !running; });

            if (!running && processQueue.empty()) break;

            if (!processQueue.empty()) {
                process = processQueue.front();
                processQueue.pop();

                runningProcesses.push_back(process);
            }
            else {
                continue;
            }
        }

        // Simulate processing by writing to a file
        std::ofstream file(process.processName + ".txt", std::ios::app);
        for (int i = 0; i < process.totalLine; ++i) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                process.currentLine = i + 1;

                auto runningIt = std::find_if(runningProcesses.begin(), runningProcesses.end(),
                    [&](const ProcessInfo& p) { return p.processName == process.processName; });
                if (runningIt != runningProcesses.end()) {
                    runningIt->currentLine = process.currentLine;
                }
            }

            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm localTime;
#ifdef _WIN32
            localtime_s(&localTime, &now);
#else
            localtime_r(&now, &localTime);
#endif
            char buffer[26];
            std::strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &localTime);
            file << "(" << buffer << ") Core:" << coreId << " \"Hello World! from " << process.processName << "\"\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        file.close();

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            process.isFinished = true;
            finishedProcesses.push_back({ process, coreId });

            runningProcesses.erase(std::remove_if(runningProcesses.begin(), runningProcesses.end(),
                [&](const ProcessInfo& p) { return p.processName == process.processName; }),
                runningProcesses.end());
        }

        cv.notify_one();
    }
}

std::vector<std::pair<ProcessInfo, int>> Scheduler::getFinishedProcesses() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return finishedProcesses;
}

std::vector<ProcessInfo> Scheduler::getRunningProcesses() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return runningProcesses; 
}
