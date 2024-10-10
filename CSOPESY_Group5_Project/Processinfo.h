#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>

struct ProcessInfo {
    std::string processName;
    int currentLine;
    int totalLine;
    std::string timeStamp;
    int arrivalTime;
    bool isFinished = false;
};

#endif
