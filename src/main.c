#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <unistd.h>
#include <sense-api.h>
#include <stdint.h>

#include "helpers.h"
#include "globals.h"
#include "main.h"
#include "sysInfo.h"
#include "logging.h"

#define staticLogLevel debug
#define BUFF_SIZE 256

// polling times (in loop ticks)
#define uptimePoll 64 // uptime is incremented once per loop automatically, but checked this often for errors
#define cpuPoll 1
#define memPoll 4
#define storagePoll 256

void pollingLoop(FILE* meminfoFp, FILE* uptimeFp, FILE* statFp,  uint16_t *ledArr);

// get info from meminfo, stat, uptime
int main(){
    initGlobals();

    FILE* meminfoFp;
    meminfoFp = fopen("/proc/meminfo" , "r");
    FILE* uptimeFp;
    uptimeFp = fopen("/proc/uptime" , "r");
    FILE* statFp;
    statFp = fopen("/proc/stat" , "r");
    
    uint16_t *ledArr = getLedArr();
    clear(ledArr);

    pollingLoop(meminfoFp,uptimeFp,statFp,ledArr);

    fclose(meminfoFp);
    fclose(uptimeFp);
    fclose(statFp);
}


void pollingLoop(FILE* meminfoFp, FILE* uptimeFp, FILE* statFp,  uint16_t *ledArr){
    char buff[BUFF_SIZE];

    unsigned long memUsage = 0;
    unsigned long memTotal = 0;
    unsigned long uptime = 0;
    
    float cpuUsage = 0.0;
    float storageUsage = 0.0;
    
    getUptime(uptimeFp,&uptime,buff,BUFF_SIZE);
    initLoopTimer();

    ulong tickNumber = 0; // monotonic
    while(1){
        
        if (tickNumber%uptimePoll == 0){
            getUptime(uptimeFp,&uptime,buff,BUFF_SIZE);
        }
        if (tickNumber%cpuPoll == 0){
            getCpuUsage(statFp,&cpuUsage,buff,BUFF_SIZE);
            percentageBar(ledArr,cpuUsage,7);
        }
        if (tickNumber%memPoll == 0){
            getMemUsage(meminfoFp,&memUsage,&memTotal,buff,BUFF_SIZE);
            percentageBar(ledArr,(float)memUsage / (float)memTotal,6);
        }
        if (tickNumber%storagePoll == 0){
            getStorageUsage(&storageUsage);
            percentageBar(ledArr,(float)storageUsage,5);
        }

        binaryBar(ledArr,uptime,0,32,1.0,1.0,1.0);
        
        printf("%li: tick...\n",tickNumber);
        sleepRemainingLoop();
        uptime+=1;
        tickNumber += 1;
    }    
}