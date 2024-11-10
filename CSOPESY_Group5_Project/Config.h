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
    int procMem;
    int numFrame;
};

struct MemoryFrame {
    int frameId;
    bool occupied;
    std::string procName;

    MemoryFrame(int id) : frameId(id), occupied(false) {}
};

#endif
