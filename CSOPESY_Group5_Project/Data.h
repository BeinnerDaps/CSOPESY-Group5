#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include "ProcessInfo.h"

class Data {

protected:
    std::vector<ProcessInfo> processList;

public:
    Data() {}

    ProcessInfo& getProcess(const std::string& processName);

    void createProcess(const std::string& processName);

    void listAllProcess();

    std::string getTimestamp() const;
};

#endif
