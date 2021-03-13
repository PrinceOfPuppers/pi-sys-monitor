#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <unistd.h>
#include "globals.h"
#include "main.h"
#include "sysInfo.h"
#include "logging.h"

#define staticLogLevel debug
#define BUFF_SIZE 256

// get info from meminfo, stat, uptime
int main(){
    initGlobals();

    FILE* meminfoFp;
    meminfoFp = fopen("/proc/meminfo" , "r");
    FILE* uptimeFp;
    uptimeFp = fopen("/proc/uptime" , "r");
    FILE* statFp;
    statFp = fopen("/proc/stat" , "r");

    char buff[BUFF_SIZE];
    

    unsigned long storageUsage = 0;
    unsigned long storageTotal = 0;

    getStorageUsage(&storageUsage,&storageTotal);

    unsigned long memUsage = 0;
    unsigned long memTotal = 0;
    unsigned long uptime = 0;
    float cpuUsage = 0.0;
    getMemUsage(meminfoFp,&memUsage,&memTotal,buff,BUFF_SIZE);
    getUptime(uptimeFp,&uptime,buff,BUFF_SIZE);
    getCpuUsage(statFp,&cpuUsage,buff,BUFF_SIZE);
    sleep(3);
    getCpuUsage(statFp,&cpuUsage,buff,BUFF_SIZE);

    fclose(meminfoFp);
    fclose(uptimeFp);
    fclose(statFp);
}
