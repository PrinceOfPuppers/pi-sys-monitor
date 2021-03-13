#include "globals.h"
#include <stdio.h>
#include "logging.h"

void initCpuStats(struct CpuStats *cpuStats){
    cpuStats->user    = &cpuStats->stats[0];
    cpuStats->nice    = &cpuStats->stats[1];
    cpuStats->system  = &cpuStats->stats[2];
    cpuStats->idle    = &cpuStats->stats[3];
    cpuStats->iowait  = &cpuStats->stats[4];
    cpuStats->irq     = &cpuStats->stats[5];
    cpuStats->softirq = &cpuStats->stats[6];
    cpuStats->steal   = &cpuStats->stats[7];

    cpuStats->total = 0;
    cpuStats->notWorking = 0;
}


Globals glob;
void initGlobals(){

    initCpuStats(&glob.cpuStats);

    if (regcomp(&glob.intRegex,"[0-9]+",REG_EXTENDED)) {
        logFatal("Failed to Compile Memory Regex");
    };
    
    //74608   2520   24433   1117073   6176   4054  0        0      0      0
    
    if (regcomp(&glob.cpuRegex,"([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)",REG_EXTENDED)) {
        logFatal("Failed to Compile Uptime Regex");
    };
}