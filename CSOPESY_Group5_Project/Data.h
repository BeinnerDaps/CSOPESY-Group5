#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include "ProcessInfo.h"

class Data {

public:
    Data() {}
    int nextProcessID = 1;
    std::vector<ProcessInfo> processList;
    ProcessInfo& getProcess(const std::string& processName);
    const std::vector<ProcessInfo>& getProcessList() const {
        return processList;
    }
    void updateProcessLine(const std::string& processName, int line);
    void listAllProcess();
    std::string getTimestamp() const;
};

#endif
