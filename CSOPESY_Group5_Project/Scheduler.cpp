#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>

Scheduler::Scheduler(const Config& config)
    : config(config), running(true) {
    for (int i = 0; i < config.numCpu; ++i) {
        coreThreads.emplace_back(&Scheduler::coreFunction, this, i + 1);
    }

    for (int i = 0; i < config.numFrame; ++i) { memoryPool.emplace_back(i); }
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

bool Scheduler::allocMemory(ProcessInfo& process) {
    int requiredFrames = config.procMem / config.frameMem;
    int freeFrames = 0;
    int startFrame = -1;
    
    // Check free frames
    for (int i = 0; i < config.numFrame; ++i) {
        if (!memoryPool[i].occupied) {
            if (freeFrames == 0) { startFrame = i; }
            freeFrames++;
            if (freeFrames == requiredFrames) { break; }
        }
        else {
            freeFrames = 0;
        }
    }

    // If not enough free frames available
    if (freeFrames < requiredFrames) {
        return false;
    }

    // Allocate the frames to the process
    for (int i = startFrame; i < startFrame + requiredFrames; ++i) {
        memoryPool[i].occupied = true;
        memoryPool[i].procName = process.processName;
    }

    // Store allocated frames
    process.procMemloc = { startFrame, startFrame + requiredFrames };
    procInMem++;
    return true;
}

void Scheduler::deallocMemory(ProcessInfo& process) {
    int startFrame = process.procMemloc.first;
    int requiredFrames = process.procMemloc.second;

    // Free the allocated frames
    for (int i = startFrame; i < requiredFrames; ++i) {
        memoryPool[i].occupied = false;
        memoryPool[i].procName = "";
    }
    procInMem--;
}

void Scheduler::Memoryreport(int cycle) {
    std::ostringstream filename;
    filename << "memory_stamp_" << cycle << ".txt";

    std::ofstream report(filename.str());
    if (!report.is_open()) return;

    // Write the timestamp and the number of processes in memory
    report << "Timestamp: " << data.getTimestamp() << "\n";
    report << "Number of processes in memory: " << procInMem << "\n";

    // Calculate external fragmentation
    int freeFrames = 0;
    for (const auto& frame : memoryPool) {
        if (!frame.occupied) freeFrames++;
    }
    report << "Total external fragmentation (KB): " << freeFrames * config.frameMem << "\n";

    // ASCII representation of memory with process names
    report << "Memory layout:\n";
    for (size_t i = 0; i < memoryPool.size(); ++i) {
        report << "[" << (memoryPool[i].occupied ? memoryPool[i].procName : " ") << "]";
        if ((i + 1) % 64 == 0) report << "\n"; // Newline every 64 frames for readability
    }
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

std::vector<ProcessInfo> Scheduler::getWaitingMemory() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return std::vector<ProcessInfo>(memoryQueue.begin(), memoryQueue.end());
}

void Scheduler::coreFunction(int coreId) {
    int cycle = 0;
    bool processAssigned = false;
    bool processCompleted = false;
    int executedCycles = 0;
    int execCycles = 0;
    int delay = 0;

    while (running) {
        ProcessInfo process(-1, "defaultProcess", 100, "Timestamp");
        processAssigned = false;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return !processQueue.empty() || !memoryQueue.empty() || !running; });
            
            if (!running && processQueue.empty() && memoryQueue.empty()) { break; }

            if (!processQueue.empty()) {
                process = processQueue.front();

                if (allocMemory(process)) {
                    processQueue.pop_front();
                } else {
                    process = memoryQueue.front();
                    memoryQueue.pop_front();
                }

            } else if (!memoryQueue.empty()) {
                process = memoryQueue.front();
                memoryQueue.pop_front();
            }

            process.assignedCore = coreId;
            runningProcesses.push_back(process);
            processAssigned = true;
            ++coresInUse;
        }

        if (!processAssigned) { continue; }

        processCompleted = false;
        executedCycles = 0;
        execCycles = (config.scheduler == "rr") ? config.quantumCycles : process.totalLine;
        delay = (config.delaysPerExec <= 1) ? 1 : config.delaysPerExec-1;

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
            deallocMemory(process);
            // std::cout << "[DEBUG] Process " << process.processName << " completed on core " << coreId << std::endl;
        }
        else if (!processCompleted && config.scheduler == "rr") {
            std::lock_guard<std::mutex> lock(queueMutex);
            memoryQueue.push_back(process);
            // std::cout << "[DEBUG] Process " << process.processName << " quantum depleted, re-queuing on core " << coreId << std::endl;
        }

        cycle++;
        Memoryreport(cycle);    

        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(config.batchProcessFreq-1));
    }
}

std::string Scheduler::getMetrics() {
    // CPU Utilization: percentage of active core time
    double cpuUtilization = (coresInUse * 100.0) / config.numCpu;

    // Cores Used and Cores Available
    int coresAvailable = config.numCpu - coresInUse;

    std::ostringstream os;
    os << "\nCPU Utilization: " << cpuUtilization << "%" << std::endl;
    os << "Cores Used: " << coresInUse << std::endl;
    os << "Cores Available: " << coresAvailable << std::endl;
    return os.str();
}


