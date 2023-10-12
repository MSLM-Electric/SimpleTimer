#ifndef __SIMPLETIMER_H_
#define __SIMPLETIMER_H_  //Protection from the headers are getting included multiple time

#include <stdint.h>

//#define USER_TICK
#define DEBUG_ON_VS
#ifndef DEBUG_ON_VS
#ifndef USER_TICK
#define HAL_INCTICK
#endif // !USER_TICK
#endif // !DEBUG

#define t100us(x) x*10
#define t10us(x) x*100

typedef struct {
	uint32_t setVal;
	uint32_t launchedTime;
	uint8_t Start;
}Timer_t;

typedef uint32_t U32_ms;
typedef uint32_t U32_us;

void LaunchSpecifiedTimer(uint32_t time, Timer_t* Timer);
void StopSpecifiedTimer(Timer_t* Timer);
uint8_t IsTimerStarted(Timer_t* Timer);
uint8_t IsTimerRinging(Timer_t* Timer);

typedef struct {
	uint32_t lastTimeFix;
	uint32_t measuredTime;
	uint16_t measureCycle;
	uint16_t _tempCycle;
	//uint8_t measureType;
}stopwatch_t;

uint32_t StopWatch(stopwatch_t* timeMeasure);
uint32_t CyclicStopWatch(stopwatch_t* timeMeasure, uint16_t Ncycle);
#endif // !__SIMPLETIMER_H_