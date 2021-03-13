#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "main.h"
#include "sysInfo.h"
#include "logging.h"

#define staticLogLevel debug
#define BUFF_SIZE 256

// get info from meminfo, stat, uptime
int main(){
    initConfig();
    SysInfo *sysinfo = initSysInfo();

    FILE* meminfo;
    meminfo = fopen("/proc/meminfo" , "r");
    FILE* uptime;
    uptime = fopen("/proc/uptime" , "r");
    FILE* stat;
    stat = fopen("/proc/stat" , "r");

    char buff[BUFF_SIZE];
    
    updateMem(meminfo,sysinfo,&buff,BUFF_SIZE);
    updateUptime(uptime,sysinfo,&buff,BUFF_SIZE);
    updateCPU(stat,sysinfo,&buff,BUFF_SIZE);
    sleep(3);
    updateCPU(stat,sysinfo,&buff,BUFF_SIZE);

    fclose(meminfo);
    fclose(uptime);
    fclose(stat);
}
