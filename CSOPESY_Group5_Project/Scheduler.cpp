// SCHEDULER

#include "Scheduler.h"

void Scheduler::start() {
    running = true;
}

void Scheduler::stop() {
    running = false;
    cv.notify_all();
}

Scheduler::Scheduler(int cores) : cores(cores) {
    start();
    // Initialize threads representing the 4 cores
    for (int i = 0; i < cores; ++i) {
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
    lock_guard<mutex> lock(queueMutex);
    processQueue.push(process);
    cv.notify_one();
}

void Scheduler::coreFunction(int coreId) {
    while (running) {
        ProcessInfo process;
        {
            unique_lock<mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return !processQueue.empty() || !running; });

            if (!running) break;

            process = processQueue.front();
            processQueue.pop();
        }

        // Simulate processing by writing to a file
        ofstream file(process.processName + ".txt", ios::app);
        
        file << "Process name: " << process.processName << endl;
        file << "Logs:" << endl;
        file << endl;

        for (process.currentLine; process.currentLine <= process.totalLine; ++process.currentLine) {
            file << "(" << process.arrivalTime << ") Core:" << coreId << " \"Hello World! from " << process.processName << "\"\n";
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        file.close();

        {
            lock_guard<mutex> lock(queueMutex);
            process.isFinished = true;
            finishedProcesses.push_back({ process, coreId });
        }
    }
}

vector<pair<Data::ProcessInfo, int>> Scheduler::getFinishedProcesses() {
    lock_guard<mutex> lock(queueMutex);
    return finishedProcesses;
}

vector<Data::ProcessInfo> Scheduler::getRunningProcesses() {
    lock_guard<std::mutex> lock(queueMutex);
    vector<ProcessInfo> runningProcesses;
    queue<ProcessInfo> tempQueue = processQueue;
    while (!tempQueue.empty()) {
        runningProcesses.push_back(tempQueue.front());
        tempQueue.pop();
    }
    return runningProcesses;
}
