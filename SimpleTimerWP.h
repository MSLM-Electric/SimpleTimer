#ifndef __SIMPLETIMERWP_H_
#define __SIMPLETIMERWP_H_  //Protection from the headers are getting included multiple time

#include <stdint.h>
#include <string.h> //only for memset

#ifndef __SIMPLETIMER_H_
#define DEBUG_ON_VS //delete this line if you don't want debug on VS!!!
#ifdef DEBUG_ON_VS
#include <Windows.h>
#endif
#define USE_REGISTERING_TIMERS_WITH_CALLBACK

#define ms_x100us(x) x*10 //1ms is  10 x 100microseconds
#define ms_x10us(x) x*100 //2ms is  200 x 10microseconds
typedef uint32_t U32_ms;
typedef uint32_t U32_us;

typedef void* (tickptr_fn)();
#endif

#ifdef USE_REGISTERING_TIMERS_WITH_CALLBACK
typedef void* (timerwpcallback_fn)(void* arg);
#define MAX_REGISTER_NUM 10
#endif // USE_REGISTERING_TIMERS_WITH_CALLBACK

enum {
	ONE_SHOT_TIMER,
	PERIODIC_TIMER
}timerType_enum;

typedef struct {
	uint32_t setVal;
	uint32_t launchedTime;
	uint8_t Start;
	tickptr_fn* ptrToTick;
	enum timerType_enum TimType;
#ifdef USE_REGISTERING_TIMERS_WITH_CALLBACK
	timerwpcallback_fn* RegisteredCallback;
	void* arg;
	void* next;
#endif // USE_REGISTERING_TIMERS_WITH_CALLBACK
}Timerwp_t;

#ifdef USE_REGISTERING_TIMERS_WITH_CALLBACK
Timerwp_t* RegisteredTimers[MAX_REGISTER_NUM];
#endif // USE_REGISTERING_TIMERS_WITH_CALLBACK

typedef struct {
	uint32_t lastTimeFix;
	uint32_t measuredTime;
	uint16_t measureCycle;
	uint16_t _tempCycle;
	//uint8_t measureType;
	tickptr_fn* ptrToTick;
}stopwatchwp_t;

void InitStopWatchWP(stopwatchwp_t* timeMeasure, tickptr_fn* SpecifyTickFunction);
void InitTimerWP(Timerwp_t* Timer, tickptr_fn* SpecifyTickFunction);
uint32_t StopWatchWP(stopwatchwp_t* timeMeasure);
uint32_t CyclicStopWatchWP(stopwatchwp_t* timeMeasure, uint16_t Ncycle);

void LaunchTimerWP(uint32_t time, Timerwp_t* Timer);
void StopTimerWP(Timerwp_t* Timer);
uint8_t IsTimerWPStarted(Timerwp_t* Timer);
uint8_t IsTimerWPRinging(Timerwp_t* Timer);
uint8_t RestartTimerWP(Timerwp_t* Timer);
#ifdef USE_REGISTERING_TIMERS_WITH_CALLBACK
uint8_t RegisterTimerCallback(Timerwp_t* Timer, timerwpcallback_fn* ThisTimerCallback, enum timerType_enum timType, tickptr_fn* SpecifyTickFunc);
uint8_t UnRegisterTimerCallback(Timerwp_t* Timer);
uint8_t RegisteredTimersCallbackHandle(Timerwp_t* Timer);
uint8_t getRegistersMaxIndex(void);
#endif // USE_REGISTERING_TIMERS_WITH_CALLBACK
#endif // !__SIMPLETIMER_H_
