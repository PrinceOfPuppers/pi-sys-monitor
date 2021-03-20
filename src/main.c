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
    
    uint16_t *ledArr = getLedArr();

    float storageUsage = 0.0;



    unsigned long memUsage = 0;
    unsigned long memTotal = 0;
    unsigned long uptime = 0;
    float cpuUsage = 0.0;

    clear(ledArr);

    //binaryBar(ledArr,10,6,8,0.0,1.0,0.0);

    

    getUptime(uptimeFp,&uptime,buff,BUFF_SIZE);

    initLoopTimer();

    ulong tickNumber = 0; // monotonic
    while(1){
        
        if (tickNumber%uptimePoll == 0){
            getUptime(uptimeFp,&uptime,buff,BUFF_SIZE);
        }
        if (tickNumber%cpuPoll == 0){
            getCpuUsage(statFp,&cpuUsage,buff,BUFF_SIZE);
            printf("cpu: %f\n",cpuUsage);
            percentageBar(ledArr,cpuUsage,7);
        }
        if (tickNumber%memPoll == 0){
            getMemUsage(meminfoFp,&memUsage,&memTotal,buff,BUFF_SIZE);
            logInfo("mem %f",(float)memUsage / (float)memTotal);
            percentageBar(ledArr,(float)memUsage / (float)memTotal,6);
        }
        if (tickNumber%storagePoll == 0){
            getStorageUsage(&storageUsage);
            logInfo("stor %f",storageUsage);
            percentageBar(ledArr,(float)storageUsage,5);
        }

        binaryBar(ledArr,uptime,0,32,1.0,1.0,1.0);
        
        printf("%li: tick...\n",tickNumber);
        sleepRemainingLoop();
        uptime+=1;
        tickNumber += 1;
    }



    
    getUptime(uptimeFp,&uptime,buff,BUFF_SIZE);
    getCpuUsage(statFp,&cpuUsage,buff,BUFF_SIZE);
    
    getCpuUsage(statFp,&cpuUsage,buff,BUFF_SIZE);
    

    fclose(meminfoFp);
    fclose(uptimeFp);
    fclose(statFp);
}
