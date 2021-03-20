#pragma once
#include <stdint.h>

void initLoopTimer();

void sleepRemainingLoop();

void percentageBar(uint16_t *ledArr, float value, int column);
void binaryBar(uint16_t *ledArr, int number, int column, int maxRows, float r, float g, float b);