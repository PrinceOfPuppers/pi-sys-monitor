#include "helpers.h"

#include <math.h>
#include <time.h>

#include <sense-api.h>
#include <sense-helpers.h>

// how long each polling loop is
#define loopTimeSec (__syscall_slong_t)1

// structs used in clock_gettime and nanosleep
struct timespec t1,t2;

// prime the loop timer with the start time
void initLoopTimer(){
    clock_gettime(CLOCK_MONOTONIC_COARSE,&t1);
}

// sleeps for enough time to make each loop exactly loopTimeSec long, called at end of loop
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

// bar graph displaying value (between 0 and 1) with its height and color
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

// shows 'number' in binary starting at the base of column 'column' and spanning rows 'maxRows'
// ie) maxRows = 16 column = 0 will make the counter span the first 2 rows of the led matrix
void binaryBar(uint16_t *ledArr, int number, int column, int maxRows, float r, float g, float b){
    float boolFloat;
    for(int i = 0; i<maxRows; i++){
        boolFloat = (float)(number & 1);
        setVal( ledArr,i,column,rgbFloatToHex(r*boolFloat,g*boolFloat,b*boolFloat) );
        number = number >> 1;
    }
}