#include "helpers.h"

#include <math.h>
#include <time.h>

#include <sense-api.h>
#include <sense-helpers.h>

//how period of each update loop
//#define loopTimeNs (__syscall_slong_t)4000000000

#define loopTimeSec (__syscall_slong_t) 1
struct timespec t1,t2;

void initLoopTimer(){
    clock_gettime(CLOCK_MONOTONIC_COARSE,&t1);
}

void sleepRemainingLoop(){
    // get time of end of loop
    clock_gettime(CLOCK_MONOTONIC_COARSE,&t2);

    //calculate sleep duration
    t2.tv_sec   = loopTimeSec - t2.tv_sec + t1.tv_sec;
    t2.tv_nsec  = loopTimeSec - (t2.tv_nsec - t1.tv_nsec)%1000000000;

    //sleep
    nanosleep(&t2,NULL);

    // get start time of next loop
    clock_gettime(CLOCK_MONOTONIC_COARSE,&t1);
}

float clampMin(float x, float min){
    return x < min ? min : x;
} 

void percentageBar(uint16_t *ledArr, float value, int column){
    float b = clampMin(cos( M_PI*(4*value )/3 ),0.0);
    float g = clampMin(cos( M_PI*(4*value + 4)/3 ),0.0);
    float r = clampMin(cos( M_PI*(4*value + 2)/3 ),0.0);

    int v = (int)roundf(value*8.0);
    int i;
    for(i = 0; i < v; i++){
        setVal(ledArr,i,column,rgbFloatToHex(r,g,b));
    }
    for(;i < 8; i++){
        setVal(ledArr,i,column,rgbIntToHex(0,0,0));
    }

    
}

// max is how many leds to override, binary can overflow one column and will continue on the next
void binaryBar(uint16_t *ledArr, int number, int column, int maxRows, float r, float g, float b){
    float boolFloat;
    for(int i = 0; i<maxRows; i++){
        boolFloat = (float)(number & 1);
        setVal( ledArr,i,column,rgbFloatToHex(r*boolFloat,g*boolFloat,b*boolFloat) );
        number = number >> 1;
    }
}