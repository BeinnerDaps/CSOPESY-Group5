#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>

struct ProcessInfo {
    std::string processName;
    int processID;
    int totalLine;
    std::string timeStamp;
    bool isFinished;
    int currentLine;
    int assignedCore;

    ProcessInfo(int id, const std::string& name, int totalLines, const std::string& timestamp, bool finished = false)
        : processID(id), processName(name), totalLine(totalLines), timeStamp(timestamp), isFinished(finished), currentLine(0), assignedCore(-1) {} // Initialize currentLine and assignedCore
};

#endif
