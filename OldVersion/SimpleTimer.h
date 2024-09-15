#ifndef __SIMPLETIMER_H_
#define __SIMPLETIMER_H_  //Protection from the headers are getting included multiple time

#include <stdint.h>

//#define USER_TICK
#define DEBUG_ON_VS
#ifndef DEBUG_ON_VS
#ifndef USER_TICK
#define HAL_INCTICK
#else
extern uint32_t someExternalTick;
#define NULL 0x0
#endif // !USER_TICK
#endif // !DEBUG

#define ms_x100us(x) x*10 //1ms is  10 x 100microseconds
#define ms_x10us(x) x*100 //2ms is  200 x 10microseconds

typedef struct {
	uint32_t setVal;
	uint32_t launchedTime;
	uint8_t Start;
}Timer_t;

typedef uint32_t U32_ms;
typedef uint32_t U32_us;

void LaunchSpecifiedTimer(uint32_t time, Timer_t* Timer);
void StopSpecifiedTimer(Timer_t* Timer);
void RestartSpecifiedTimer(Timer_t* Timer);
uint8_t IsTimerStarted(Timer_t* Timer);
uint8_t IsTimerRinging(Timer_t* Timer);

typedef struct {
	uint32_t lastTimeFix;
	uint32_t measuredTime;
	uint16_t measureCycle;
	uint16_t _tempCycle;
	//uint8_t measureType;
}stopwatch_t;

typedef void* (tickptr_fn)();
typedef void* (timerwpcallback_fn)(void* arg);

typedef struct {
	uint32_t setVal;
	uint32_t launchedTime;
	uint8_t Start;
	tickptr_fn* ptrToTick;
}SpecTimer_t;

typedef struct {
	uint32_t lastTimeFix;
	uint32_t measuredTime;
	uint16_t measureCycle;
	uint16_t _tempCycle;
	//uint8_t measureType;
	tickptr_fn* ptrToTick;
}specStopwatch_t;

uint32_t StopWatch(stopwatch_t* timeMeasure);
uint32_t CyclicStopWatch(stopwatch_t* timeMeasure, uint16_t Ncycle);
uint32_t StopWatchSpec(specStopwatch_t* timeMeasure);

uint32_t CyclicStopWatchSpec(specStopwatch_t* timeMeasure, uint16_t Ncycle);

void LaunchTimerSpec(uint32_t time, SpecTimer_t* Timer);
void StopTimerSpec(SpecTimer_t* Timer);
uint8_t IsTimerSpecStarted(SpecTimer_t* Timer);
uint8_t IsTimerSpecRinging(SpecTimer_t* Timer);

#endif // !__SIMPLETIMER_H_
