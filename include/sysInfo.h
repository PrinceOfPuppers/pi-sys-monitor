#include <stdio.h>
#include <sys/statvfs.h>
#pragma once

struct CpuStats{
    // parsed from /proc/stat
    int stats[8];

    // named refrences to elements stats array (done this way to fill stats in loop)
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

    // coefficent from 0 to 1
    float cpuUsage;
};

struct SysInfo{
    // RAM in bytes
    unsigned long memTotal; 
    unsigned long memAvalible;

    // Storage in bytes
    unsigned long storageTotal;
    unsigned long storageAvalible;

    // uptime in seconds
    int uptime;

    struct CpuStats cpuStats;

    // stored for reuse 
    struct statvfs vfs;
};
typedef struct SysInfo SysInfo;

SysInfo *initSysInfo();


void updateStorage(SysInfo* sysInfo);

void updateMem(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size);

void updateCPU(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size);

void updateUptime(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size);
