#include "sysInfo.h"
#include "config.h"
#include "logging.h"

#include <stdlib.h>

SysInfo *initSysInfo(){
    SysInfo *sysInfo = malloc(sizeof(SysInfo));
    //struct CpuStats *cpuStats = malloc(sizeof(CpuStats));
    sysInfo->cpuStats.notWorking = 0;
    sysInfo->cpuStats.total = 0;

    sysInfo->cpuStats.user = &sysInfo->cpuStats.stats[0];
    sysInfo->cpuStats.nice = &sysInfo->cpuStats.stats[1];
    sysInfo->cpuStats.system = &sysInfo->cpuStats.stats[2];
    sysInfo->cpuStats.idle = &sysInfo->cpuStats.stats[3];
    sysInfo->cpuStats.iowait = &sysInfo->cpuStats.stats[4];
    sysInfo->cpuStats.irq = &sysInfo->cpuStats.stats[5];
    sysInfo->cpuStats.softirq = &sysInfo->cpuStats.stats[6];
    sysInfo->cpuStats.steal = &sysInfo->cpuStats.stats[7];

    return sysInfo;
}


void updateMem(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size){
    logDebug("Updating Memory Stats");
    int start,end,notSuccess;
    char *numStr;

    regmatch_t matchGroup[1];


    // Get MemTotal
    fgets(buffer,buffer_size-1,fp);

    notSuccess = regexec(&cfg.memRegex,buffer, 1, matchGroup, 0);
    if(notSuccess){
        logFatal("Unable to Find MemTotal");
    }


    start = matchGroup[0].rm_so;
    end = matchGroup[0].rm_eo;

    numStr = buffer+start;

    buffer[end] = '\0';
    sysInfo->memTotal = atoi(numStr);

    // Get MemFree
    fgets(buffer,buffer_size-1,fp);

    notSuccess = regexec(&cfg.memRegex,buffer, 1, matchGroup, 0);
    if(notSuccess){
        logFatal("Unable to Find MemFree");
    }
    
    start = matchGroup[0].rm_so;
    end = matchGroup[0].rm_eo;

    numStr = buffer+start;

    buffer[end] = '\0';
    sysInfo->memFree = atoi(numStr);

    logInfo("Memory Usage: %ikb / %ikb", sysInfo->memFree, sysInfo->memTotal);

    fseek(fp, 0, SEEK_SET);
}


void updateUptime(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size){
    logDebug("Updating Uptime");
    int start,end,notSuccess;
    char *numStr;

    regmatch_t matchGroup[1];

    fgets(buffer,buffer_size-1,fp);

    notSuccess = regexec(&cfg.uptimeRegex,buffer, 1, matchGroup, 0);
    if(notSuccess){
        logFatal("Unable to Find uptime");
    }

    start = matchGroup[0].rm_so;
    end = matchGroup[0].rm_eo;

    numStr = buffer+start;

    buffer[end] = '\0';
    
    sysInfo->uptime = atoi(numStr);

    logInfo("System Uptime: %i",sysInfo->uptime);

    fseek(fp, 0, SEEK_SET);
}


// /proc/stat syntax
//     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
//cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
void updateCPU(FILE* fp, SysInfo* sysInfo, char *buffer, size_t buffer_size){
    logDebug("Updating CPU");
    int start,end,notSuccess;
    char *numStr;

    regmatch_t matchGroup[9];

    fgets(buffer,buffer_size-1,fp);

    notSuccess = regexec(&cfg.cpuRegex,buffer, 9, matchGroup, 0);
    if(notSuccess){
        logFatal("Unable to Find CPU Stats");
    }

    int *pntr;
    for(int i = 1; i<9; i++){ // we skip first match because we are only intrested in the sub matches
        start = matchGroup[i].rm_so;
        end = matchGroup[i].rm_eo;

        numStr = buffer+start;
        buffer[end] = '\0';


        sysInfo->cpuStats.stats[i-1] = atoi(numStr);
    }

    // actual calculations
    int notWorking = *sysInfo->cpuStats.idle + *sysInfo->cpuStats.iowait;
    int working = *sysInfo->cpuStats.user + *sysInfo->cpuStats.nice + *sysInfo->cpuStats.system + *sysInfo->cpuStats.irq + *sysInfo->cpuStats.softirq + *sysInfo->cpuStats.steal;

    int total = notWorking+working;

    int dtotal = total - sysInfo->cpuStats.total;
    int dnotWorking = notWorking - sysInfo->cpuStats.notWorking;

    float usage = (float)(dtotal - dnotWorking)/ (float)dtotal;
    logInfo("CPU Usage: %f%%",usage*100);

    sysInfo->cpuStats.total = total;
    sysInfo->cpuStats.notWorking = notWorking;

    fseek(fp, 0, SEEK_SET);
}