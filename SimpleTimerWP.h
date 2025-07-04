#ifndef __SIMPLETIMERWP_H_
#define __SIMPLETIMERWP_H_
#ifdef __cplusplus
extern "C" {
#endif // !_cplusplus

#include <stdint.h>
#include <string.h> //only for memset

#ifndef __SIMPLETIMER_H_
#define DEBUG_ON_VS //delete this line if you don't want debug on VS!!!
#ifdef DEBUG_ON_VS
#include <Windows.h>
#endif
//#define MINIMAL_CODESIZE
#ifndef MINIMAL_CODESIZE
#define USE_REGISTERING_TIMERS_WITH_CALLBACK
#endif // !MINIMAL_CODESIZE

#define x1ms   //just for beautyfying and readability code
#define x10ms  
#define x100ms
#define x1s
#define x1us
#define x10us
#define x100us //just for beautyfying and readability code

typedef uint32_t U32_ms;
typedef uint32_t U32_us;

#ifndef TIMER_TICK_16BIT_EN
#define TIMER_TICK_16BIT_EN 0
#endif // !TIMER_TICK_16BIT_EN
#ifndef TIMER_TICK_32BIT_EN
#define TIMER_TICK_32BIT_EN 1
#endif // !TIMER_TICK_32BIT_EN
#ifndef TIMER_TICK_8BIT_EN
#define TIMER_TICK_8BIT_EN 0 //!not implemented yet
#endif // !TIMER_TICK_8BIT_EN


#if TIMER_TICK_16BIT_EN
typedef uint16_t TimerBaseType;
#elif TIMER_TICK_32BIT_EN
typedef uint32_t TimerBaseType;
#endif // 

#ifndef MINIMAL_CODESIZE
typedef void* (tickptr_fn)();
#else
typedef void* tickptr_fn; //just for compatiblity
#endif // !MINIMAL_CODESIZE
#endif // !__SIMPLETIMER_H_

#ifdef MINIMAL_CODESIZE
extern TimerBaseType someExternalTick;
#endif // !MINIMAL_CODESIZE


#if defined(USE_REGISTERING_TIMERS_WITH_CALLBACK) && !defined(MINIMAL_CODESIZE)
typedef void* (timerwpcallback_fn)(void* arg);
#define MAX_REGISTER_NUM 10
#endif // !USE_REGISTERING_TIMERS_WITH_CALLBACK && MINIMAL_CODESIZE

typedef enum {
	ONE_SHOT_TIMER,
	PERIODIC_TIMER
}timerType_enum;

typedef struct {
	TimerBaseType setVal;
	TimerBaseType launchedTime;
	uint8_t Start;
#ifndef MINIMAL_CODESIZE
    timerType_enum TimType;
	tickptr_fn* ptrToTick;
#ifdef USE_REGISTERING_TIMERS_WITH_CALLBACK
	timerwpcallback_fn* RegisteredCallback;
	void* arg;
	void* next;
#endif // !USE_REGISTERING_TIMERS_WITH_CALLBACK
#endif // !MINIMAL_CODESIZE
}Timert_t;

typedef struct {
	TimerBaseType setVal;
	TimerBaseType launchedTime;
	uint8_t Start;
#ifndef MINIMAL_CODESIZE
	timerType_enum TimType;
#endif // !MINIMAL_CODESIZE
}SimpleTimer_t;

#if defined(USE_REGISTERING_TIMERS_WITH_CALLBACK) && !defined(MINIMAL_CODESIZE)
extern Timert_t* RegisteredTimers[MAX_REGISTER_NUM];
#endif // !USE_REGISTERING_TIMERS_WITH_CALLBACK

typedef struct {
	uint32_t lastTimeFix;
	uint32_t measuredTime;
	uint16_t measureCycle;
	uint16_t _tempCycle;
	//uint8_t measureType;
	tickptr_fn* ptrToTick;
}stopwatchwp_t;

void InitTimerWP(Timert_t* Timer, tickptr_fn* SpecifyTickFunction);
void InitTimerGroup(Timert_t* ArrTimers, tickptr_fn* SpecifyTickFunction, uint8_t qntyTimers, TimerBaseType setVals);
#ifndef MINIMAL_CODESIZE
void InitStopWatchWP(stopwatchwp_t* timeMeasure, tickptr_fn* SpecifyTickFunction);
void InitStopWatchGroup(stopwatchwp_t* stopwatchArr, tickptr_fn* SpecifyTickFunction, uint8_t qnty);
uint32_t StopWatchWP(stopwatchwp_t* timeMeasure);
uint32_t CyclicStopWatchWP(stopwatchwp_t* timeMeasure, uint16_t Ncycle);
#endif // !MINIMAL_CODESIZE

void LaunchTimerWP(TimerBaseType time, Timert_t* Timer);
void StopTimerWP(Timert_t* Timer);
void StopTimerGroup(Timert_t* ArrTimers, uint8_t qntyTimers);
uint8_t IsTimerWPStarted(Timert_t* Timer);
uint8_t IsTimerWPRinging(Timert_t* Timer);
uint8_t IsTimerRingingKnowByRef(SimpleTimer_t *Timer, TimerBaseType asRef);
uint8_t RestartTimerWP(Timert_t* Timer);
uint8_t RestartTimerGroup(Timert_t* ArrTimers, uint8_t qntyTimers);
void catchBreakPoint(TimerBaseType *var); //Click to set breakpoint there where it called when debugging
#if defined(USE_REGISTERING_TIMERS_WITH_CALLBACK) && !defined(MINIMAL_CODESIZE)
uint8_t RegisterTimerCallback(Timert_t* Timer, timerwpcallback_fn* ThisTimerCallback, timerType_enum timType, tickptr_fn* SpecifyTickFunc);  //RegisterTimerWithCallbackToList() sounds better
uint8_t UnRegisterTimerCallback(Timert_t* Timer);                                                                                            //UnRegisterTimerWithCallbackFromList()
uint8_t RegisteredTimersCallbackHandle(Timert_t* Timer);  ///HandleRegisteredTimersOnList() RegisteredTimersFromListHandle()
uint8_t getRegisterTimersMaxIndex(void);
#endif // USE_REGISTERING_TIMERS_WITH_CALLBACK
//#define USING_RTOS
#ifdef USING_RTOS
void TaskYieldWithinSpecifiedTime(const uint32_t time, Timert_t* Timer);
#endif

#ifdef __cplusplus
}
#endif // !_cplusplus

#endif // !__SIMPLETIMERWP_H_