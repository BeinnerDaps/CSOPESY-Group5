#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>

struct ProcessInfo {
    std::string processName;
    int currentLine = 0;
    int totalLine;
    std::string timeStamp;
    bool isFinished = false;
};


#endif
