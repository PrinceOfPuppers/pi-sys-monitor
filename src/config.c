#include "config.h"
#include <stdio.h>
#include "logging.h"

Config cfg;
void initConfig(){
    if (regcomp(&cfg.memRegex,"[0-9]+",REG_EXTENDED)) {
        logFatal("Failed to Compile Memory Regex");
    };
    
    if (regcomp(&cfg.uptimeRegex,"[0-9]+",REG_EXTENDED)) {
        logFatal("Failed to Compile Uptime Regex");
    };

    //74608   2520   24433   1117073   6176   4054  0        0      0      0
    
    if (regcomp(&cfg.cpuRegex,"([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)",REG_EXTENDED)) {
        logFatal("Failed to Compile Uptime Regex");
    };
}