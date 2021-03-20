#include "sysInfo.h"
#include "globals.h"
#include "logging.h"


#include <stdlib.h>

unsigned long readValFromLine(FILE *fp, regmatch_t* matchGroup, char *buffer,size_t buffer_size){
    // Get MemTotal
    
    fflush(fp);
    fgets(buffer,buffer_size-1,fp);

    if(regexec(&glob.intRegex,buffer, 1, matchGroup, 0)){
        logErr("Unable to Find Value");
        return 0lu;
    }


    int start = matchGroup[0].rm_so;
    int end = matchGroup[0].rm_eo;

    char *valPntr = buffer+start;

    buffer[end] = '\0';
    return atol(valPntr);    
}

void getStorageUsage(float *usage){
    if (statvfs("/", &glob.vfs) != 0) {
        logErr("Unable to parse VFS");
        return;
    }

    *usage = (float)(glob.vfs.f_blocks - glob.vfs.f_bfree) / (float)glob.vfs.f_blocks;
    logInfo("Storage Usage: %f",*usage);
}

void getMemUsage(FILE* fp, unsigned long *usage, unsigned long *total, char *buffer, size_t buffer_size){
    regmatch_t matchGroup[1];
    
    // Get MemTotal
    *total = readValFromLine(fp,matchGroup,buffer,buffer_size);

    // Get MemFree
    unsigned long free = readValFromLine(fp,matchGroup,buffer,buffer_size);
    // Skip MemAvalible
    fgets(buffer,buffer_size-1,fp);

    unsigned long buffers = readValFromLine(fp,matchGroup,buffer,buffer_size);
    unsigned long cached = readValFromLine(fp,matchGroup,buffer,buffer_size);

    // convert from kibibytes to bytes
    *usage = (*total - free - buffers - cached)*1024;
    *total *= 1024;
    logInfo("Memory Usage: %lubytes / %lubytes", *usage, *total);

    fseek(fp, 0, SEEK_SET);
}


void getUptime(FILE* fp, unsigned long* uptime, char *buffer, size_t buffer_size){

    regmatch_t matchGroup[1];
    
    *uptime = readValFromLine(fp,matchGroup,buffer,buffer_size);

    logInfo("System Uptime: %i",*uptime);

    fseek(fp, 0, SEEK_SET);
}


// /proc/stat syntax
//     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
//cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
void getCpuUsage(FILE* fp, float* usage, char *buffer, size_t buffer_size){
    int start,end,notSuccess;
    char *numStr;

    regmatch_t matchGroup[9];

    fflush(fp);
    fgets(buffer,buffer_size-1,fp);

    notSuccess = regexec(&glob.cpuRegex,buffer, 9, matchGroup, 0);
    if(notSuccess){
        logErr("Unable to Find CPU Stats");
        return;
    }

    for(int i = 1; i<9; i++){ // we skip first match because we are only intrested in the sub matches
        start = matchGroup[i].rm_so;
        end = matchGroup[i].rm_eo;

        numStr = buffer+start;
        buffer[end] = '\0';


        glob.cpuStats.stats[i-1] = atoi(numStr);
    }

    // actual calculations
    int notWorking = *glob.cpuStats.idle + *glob.cpuStats.iowait;
    int working = *glob.cpuStats.user + *glob.cpuStats.nice + *glob.cpuStats.system + *glob.cpuStats.irq + *glob.cpuStats.softirq + *glob.cpuStats.steal;

    int total = notWorking+working;

    int dtotal = total - glob.cpuStats.total;
    int dnotWorking = notWorking - glob.cpuStats.notWorking;

    *usage = (float)(dtotal - dnotWorking)/ (float)dtotal;

    logInfo("CPU Usage: %f%%",*usage*100.0,(float)dtotal);

    glob.cpuStats.total = total;
    glob.cpuStats.notWorking = notWorking;

    fseek(fp, 0, SEEK_SET);
}