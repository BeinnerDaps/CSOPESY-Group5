#include "Scheduler.h"

Scheduler::Scheduler(const Config& config) : config(config), running(true), generator(false) {
    clearJsonFile("Backing_Store.json");
    clearJsonFile("BS_Logs.json");
    
    for (int i = 0; i < config.numCpu; ++i) {
        coreThreads.emplace_back(&Scheduler::coreFunction, this, i + 1);
    }

    for (int i = 0; i < config.numFrame; ++i) { memoryPool.emplace_back(i); }
}

Scheduler::~Scheduler() {
    stop();
    running = false;
    cv.notify_all();

    for (auto& thread : coreThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    std::cout << "[Logs] Memory reports are found in memory_reports folder" << std::endl;
}

void Scheduler::start() {
    generator = true;
    generatorThread = std::thread(&Scheduler::processGenerator, this);
}

void Scheduler::stop() {
    generator = false;
    cv.notify_all();

    if (generatorThread.joinable()) {
        generatorThread.join();
    }
}

// Continuously Generate and Add New Processes to Memory
void Scheduler::processGenerator() {
    while (generator) {
        if (processID > 50) { continue; }
        ProcessInfo process(
            processID++,
            "process" + std::to_string(processID),
            data.getTimestamp(),
            getRandomInt(config.procMinMem, config.procMaxMem),
            0,
            getRandomInt(config.minIns, config.maxIns),
            false
        );
        
        addProcess(process); 
        
        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomInt(100, 1000)));
    }
    std::cout << "[Scheduler-stop] Process generation stopped." << std::endl;
}

void Scheduler::addProcess(ProcessInfo& process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push_back(process);
    cv.notify_one();
}

bool Scheduler::allocMemory(ProcessInfo& process) {
    int totalPages = process.procMemsize / config.frameMem /*+ (process.procMemsize % config.frameMem != 0)*/;

    for (const auto& frame : memoryPool) {
        if (frame.occupied) {
            std::wcout << frame.procID << " ";
        }
        else {
            std::wcout << frame.occupied << " ";
        }
    }
    std::cout << "\n";

    std::vector<int> freeFrames;
    {
        std::unique_lock<std::mutex> lock(memPoolMutex);

        // Check free frames
        for (const auto& frame : memoryPool) {
            if (!frame.occupied) {
                freeFrames.push_back(frame.frameId);
                if (freeFrames.size() == totalPages) { break; }
            }
        }
    }

    // If not enough free frames available
    if (freeFrames.size() < totalPages) { return false; }

    // Allocate the frames to the process
    {
        std::unique_lock<std::mutex> lock(memPoolMutex);

        for (int frame : freeFrames) {
            process.pageTable.push_back(frame);
            memoryPool[frame].occupied = true;
            memoryPool[frame].procID = process.processID;
            memoryPool[frame].procName = process.processName;
            memoryPool[frame].procAge = counter;
        }
    }

    counter++;
    procInMem++;

    std::cout << "[DEBUG] Alloc: " << process.processName << std::endl;
    return true;
}

void Scheduler::deallocMemory(ProcessInfo& process) {
    std::unique_lock<std::mutex> lock(memPoolMutex);

    for (int frame : process.pageTable) {
        memoryPool[frame].occupied = false;
        memoryPool[frame].procName = std::string();
        memoryPool[frame].procAge = INT_MAX;
    }
    
    process.pageTable.clear();

    writeBackingStore(process);
    std::cout << "[DEBUG] [Stored] " << process.processName << " " << std::endl;
    procInMem--;
}

void Scheduler::evictOldestProc() {
    std::string oldest = std::string();
    int minAge = INT_MAX;

    {
        std::unique_lock<std::mutex> lock(memPoolMutex);

        for (const auto& frame : memoryPool) {
            if (frame.occupied && frame.procAge < minAge) {
                minAge = frame.procAge;
                oldest = frame.procName;
            }
        }
    }
    
    auto it = std::find_if(memoryQueue.begin(), memoryQueue.end(),
        [&oldest](const ProcessInfo& process) {
            return process.processName == oldest;
        });  

    if (it != memoryQueue.end()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        deallocMemory(*it);
        memoryQueue.erase(it);
    }
}

// Open the file in write mode to clear its contents
void Scheduler::clearJsonFile(const std::string& fileName) {

    std::ofstream file(fileName, std::ofstream::trunc);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open file: " << fileName << std::endl;
        return;
    }

    json emptyArray = json::array();
    file << emptyArray.dump();
    file.close();

    std::cout << "[DEBUG] JSON file cleared: " << fileName << std::endl;
}

void Scheduler::writeBackingStore(const ProcessInfo& process) {
    
    try {
        std::unique_lock<std::mutex> lock(fileMutex);

        std::ifstream readFile("Backing_Store.json");
        if (!readFile.is_open()) { throw std::runtime_error("Failed to open Backing_Store.json."); }

        json BS_process = json::array();
        readFile >> BS_process;
        readFile.close();

        json processDetails;
        processDetails["processID"] = process.processID;
        processDetails["processName"] = process.processName;
        processDetails["arrivalTime"] = process.arrivalTime;
        processDetails["procMemsize"] = process.procMemsize;
        processDetails["currentLine"] = process.currentLine;
        processDetails["totalLine"] = process.totalLine;
        processDetails["isFinished"] = process.isFinished;

        BS_process.push_back(processDetails);

        std::ofstream updateFile("Backing_Store.json");
        if (!updateFile.is_open()) { throw std::runtime_error("Failed to open Backing_Store.json."); }
            
        updateFile << BS_process.dump(4);
        updateFile.close();

        logBackingStore(process, "[Stored]");
    } 
    catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

ProcessInfo Scheduler::readBackingStore() {
    ProcessInfo restoredProcess(0, std::string(), std::string(), 0, 0, 0, false);

    fs::path filePath("Backing_Store.json");
    if (!fs::file_size(filePath)) { return restoredProcess; }

    try {
        std::unique_lock<std::mutex> lock(fileMutex);

        std::ifstream readFile("Backing_Store.json");
        if (!readFile.is_open()) { throw std::runtime_error("Failed to open Backing_Store.json."); }

        json BS_process = json::array();
        readFile >> BS_process;
        readFile.close();

        if (BS_process.empty()) { return restoredProcess; }

        // Retrieve the first process from BS
        auto& processDetails = BS_process[0];

        restoredProcess.processID = processDetails["processID"];
        restoredProcess.processName = processDetails["processName"];
        restoredProcess.arrivalTime = processDetails["arrivalTime"];
        restoredProcess.procMemsize = processDetails["procMemsize"];
        restoredProcess.currentLine = processDetails["currentLine"];
        restoredProcess.totalLine = processDetails["totalLine"];
        restoredProcess.isFinished = processDetails["isFinished"];

        BS_process.erase(BS_process.begin());

        // Save the updated list back to the JSON file
        std::ofstream updateFile("Backing_Store.json");
        if (!updateFile.is_open()) { throw std::runtime_error("Failed to write to Backing_Store.json."); }

        updateFile << BS_process.dump(4);
        updateFile.close();

        logBackingStore(restoredProcess, "[Retrieved]");
    }
    catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    } 

    std::cout << "[DEBUG] " << "[Retrieved] " << restoredProcess.processName << std::endl;
    return restoredProcess;
}

void Scheduler::logBackingStore(const ProcessInfo& process, const std::string& action) {
    try {
        std::ofstream logFile("BS_Logs.json", std::ios::app);

        if (!logFile.is_open()) { throw std::runtime_error("Failed to open BS_Logs.json."); }

        json logEntry;
        logEntry["status"] = action;
        logEntry["timestamp"] = data.getTimestamp();
        logEntry["processID"] = process.processID;
        logEntry["processName"] = process.processName;
        logEntry["currentLine"] = process.currentLine;
        logEntry["totalLine"] = process.totalLine;

        logFile << logEntry.dump(4);
        logFile.close();
    }
    catch (const std::exception&) {
        std::cout << "[ERROR] Failed to open BackingStoreLogs.json" << std::endl;
    }
}

void Scheduler::checkWaiting() {
    ProcessInfo process(0, "null", "null", 0, 0, 0, false);
    
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        if (!processQueue.empty()) {
            process = processQueue.front();
            processQueue.pop_front();
        }
        else {
            process = readBackingStore();
        }
    }

    if (!process.processID) { return; }  

    while (!allocMemory(process)) {
        evictOldestProc();
    }

    {
        std::unique_lock<std::mutex> lock(queueMutex);
        memoryQueue.push_back(process);
    }
}

void Scheduler::coreFunction(int coreId) {
    int cycle = 0;
    bool processAssigned;
    bool processCompleted = false;
    int executedCycles = 0;
    int execCycles = 0;
    int delay = 0;

    while (running) {
        ProcessInfo process(0, std::string(), std::string(), 0, 0, 0, false);
        processAssigned = false;

        checkWaiting();

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            cv.wait(lock, [this]() {
                return !processQueue.empty() || !readBackingStore().processID || !memoryQueue.empty();
            });

            if (!memoryQueue.empty()) {
                process = memoryQueue.front();
                memoryQueue.pop_front();

                process.assignedCore = coreId;
                ++coresInUse;
                runningProcesses.push_back(process);
                processAssigned = true;

                //std::cout << "[DEBUG] Core " << coreId << ": " << process.processName << std::endl;
            }
        }

        if (!processAssigned) { continue; }
        
        processCompleted = false;
        executedCycles = 0;
        execCycles = (config.scheduler == "rr") ? config.quantumCycles : process.totalLine;
        delay = (config.delaysPerExec <= 1) ? 1 : config.delaysPerExec;

        while (executedCycles < execCycles && process.currentLine < process.totalLine) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            process.currentLine++;
            executedCycles++;
            cycle++;

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

        Memoryreport(cycle);
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(config.batchProcessFreq));
    }
}

void Scheduler::Memoryreport(int cycle) {
    return;
    //std::string folderName = "memory_reports";

    //fs::create_directory(folderName);

    //std::ostringstream filename;

    //filename << folderName << "/memory_stamp_" << cycle << ".txt";

    //std::ofstream report(filename.str());

    //if (!report.is_open()) return;

    //// Write the timestamp
    //report << "======================================\n";
    //report << "Timestamp: " << data.getTimestamp() << "\n";

    //// Number of processes in memory
    //report << "Number of processes in memory: " << procInMem << "\n";

    //// Calculate total external fragmentation
    //int freeMemory = 0;
    //int externalFragmentation = 0;
    //int currentFreeBlock = 0;
    //for (const auto& frame : memoryPool) {
    //    if (!frame.occupied) {
    //        currentFreeBlock += config.frameMem;
    //    }
    //    else if (currentFreeBlock > 0 && currentFreeBlock < config.procMem) {
    //        externalFragmentation += currentFreeBlock;
    //        currentFreeBlock = 0;
    //    }
    //}
    //// Add last free block if it qualifies as external fragmentation
    //if (currentFreeBlock > 0 && currentFreeBlock < config.procMem) {
    //    externalFragmentation += currentFreeBlock;
    //}
    //report << "Total external fragmentation in KB: " << (externalFragmentation / 1024) << "\n\n";

    //// Print the memory layout in ASCII format
    //report << "----end---- = " << config.overallMem << "\n";
    //int address = config.overallMem;
    //for (int i = memoryPool.size() - 1; i >= 0; i -= (memoryPool.size()/4)) {
    //    if (memoryPool[i].occupied) {
    //        report << "\n" << address << "\nP" << memoryPool[i].procName;
    //    }
    //    else {
    //        report << "\n" << address << "\n";
    //    }
    //    address -= config.procMem;
    //}

    //report << "\n----start---- = 0\n";
    //report << "======================================\n";

    //// Close file
    //report.close();

}

ProcessInfo& Scheduler::getProcess(const std::string& name) {
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

int Scheduler::getRandomInt(int floor, int ceiling) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(floor, ceiling);
    return dist(gen);
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

