#include <stdio.h>
#pragma once

struct CpuStats{
    // parsed from /proc/stat
    int stats[8];

    // named refrences to elements stats array
    int *user;
    int *nice;
    int *system;
    int *idle;
    int *iowait;
    int *irq;
    int *softirq;
    int *steal;

    // caluclated
    int notWorking;
    int total;
    int percentUsage;
};

struct SysInfo{
    int memTotal;
    int memFree;

    int uptime;
    struct CpuStats cpuStats;
};
typedef struct SysInfo SysInfo;

SysInfo *initSysInfo();


void updateMem(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size);

void updateCPU(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size);

void updateUptime(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size);
