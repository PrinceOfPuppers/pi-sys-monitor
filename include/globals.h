#pragma once

#include <regex.h>
#include <sys/statvfs.h>

struct CpuStats{
    // parsed from /proc/stat
    int stats[8];

    // named refrences to elements stats array (done this way to fill stats in loop 
    // but not have all the calculations look esoteric)
    int *user;
    int *nice;
    int *system;
    int *idle;
    int *iowait;
    int *irq;
    int *softirq;
    int *steal;

    // calculated
    int total;
    int notWorking;
};
// sets up all the refrences to stats[8]
void initCpuStats(struct CpuStats *);


struct Globals{
    regex_t intRegex;
    regex_t cpuRegex;

    // all stored for reuse
    struct CpuStats cpuStats;
    struct statvfs vfs;
};
typedef struct Globals Globals;

void initGlobals();

extern Globals glob;