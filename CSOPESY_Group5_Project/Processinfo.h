#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>

class ProcessInfo {

public:
    int processID;
    std::string processName;
    std::string arrivalTime;
    int procMemsize;
    int currentLine;
    int totalLine;
    bool isFinished;
    int assignedCore;
    std::vector<int> pageTable;

    ProcessInfo(
        int id,
        const std::string& name,
        const std::string& arrivetime,
        int size,
        int currentline,
        int totallines,
        bool finished
        
    ) : processID(id),
        processName(name),
        arrivalTime(arrivetime),
        procMemsize(size),
        currentLine(currentline),
        totalLine(totallines),
        isFinished(finished),
        assignedCore(-1),
        pageTable{}
    {}
};

#endif
