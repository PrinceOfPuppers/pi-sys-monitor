#pragma once

#include <regex.h>

struct Config{
    regex_t memRegex;
    regex_t uptimeRegex;
    regex_t cpuRegex;
};
typedef struct Config Config;

void initConfig();

extern Config cfg;