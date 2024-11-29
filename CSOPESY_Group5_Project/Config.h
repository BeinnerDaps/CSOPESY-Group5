#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    int numCpu;
    std::string scheduler;
    int quantumCycles;
    int batchProcessFreq;
    int minIns;
    int maxIns;
    int delaysPerExec;
    
    int overallMem;
    int frameMem;
    int procMinMem;
    int procMaxMem;
    int numFrame;
};

struct MemoryFrame {
    int frameId;
    bool occupied;
    std::string procName;
    int procID;
    int procAge;

    MemoryFrame(int id) 
        : frameId(id),
          occupied(false),
          procID(0),
          procName(std::string()),
          procAge(INT_MAX)
    {}
};

#endif
