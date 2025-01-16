#include <stdint.h>
#include "SimpleTimerWP.h"
#ifdef DEBUG_ON_VS
#include <stdio.h>
#include <Windows.h>
#endif // DEBUG_ON_VS

typedef struct {
	Timert_t Timers[5];
	uint32_t state;
}SomeProcess_t;
SomeProcess_t SomeProcess;

Timert_t Timer3s = { 0 },
Timer2s = { 0 }, 
Timer1s = { 0 }, 
Timer5sOneS = { 0 },
Timer10sOne = {0},
Timer15s = {0},
ms_Delay = { 0 }, Delay10s = { 0 };
stopwatchwp_t Test15s = { 0 };
static stopwatchwp_t timeMeasure[5];
uint8_t testVarcmd = 0;
extern Timert_t* RegisteredTimers[];
static void TimersCallback(void* arg);
static void AnotherCallback(void* arg);
static void ThirdCallback(void* arg);
void InterruptHardwareTimerImmitation(void);
static void simulateTick(void);
static uint32_t getSimulatedTick(void);
static void LoadTooMuchTimers(void);
static void StressTests(void);

void InitTimersRegistration(void)
{
#ifndef MINIMAL_CODESIZE
	RegisterTimerCallback(&Timer1s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer2s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer3s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer5sOneS, (timerwpcallback_fn*)TimersCallback, ONE_SHOT_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer10sOne, (timerwpcallback_fn*)AnotherCallback, ONE_SHOT_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer15s, (timerwpcallback_fn*)ThirdCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	Timer15s.arg = &testVarcmd;
	Test15s.ptrToTick = (tickptr_fn*)GetTickCount;
	InitTimerGroup(SomeProcess.Timers, (tickptr_fn*)GetTickCount, sizeof(SomeProcess.Timers) / sizeof(Timert_t), (U32_ms)1000);
#endif // !MINIMAL_CODESIZE
	LaunchTimerWP(1000 x1ms, &Timer1s);
	LaunchTimerWP(2000 x1ms, &Timer2s);
	LaunchTimerWP(3000 x1ms, &Timer3s);
	LaunchTimerWP(5000 x1ms, &Timer5sOneS);
	LaunchTimerWP((U32_ms)10000, &Timer10sOne);
	LaunchTimerWP(15000 x1ms, &Timer15s);
	return;
}

int main(void)
{
	uint8_t stopwatchesQnty = sizeof(timeMeasure) / sizeof(stopwatchwp_t);
#ifndef MINIMAL_CODESIZE
	InitStopWatchGroup(timeMeasure, (tickptr_fn*)GetTickCount, stopwatchesQnty);
	InitTimerWP(&ms_Delay, (tickptr_fn*)GetTickCount);  //its just simple timer/delay. It func. is just do some delay operation not precisiously and without blocking
	InitTimerWP(&Delay10s, (tickptr_fn*)GetTickCount);
#endif // !MINIMAL_CODESIZE
	InitTimersRegistration();
	LaunchTimerWP((U32_ms)10, &ms_Delay); //10ms delay
	LaunchTimerWP((U32_ms)10000, &Delay10s);
	while (1)
	{		
		if (IsTimerWPRinging(&ms_Delay)) {
			RestartTimerWP(&ms_Delay);
			//printf("%u ms measured val is: %u\n", ms_Delay.setVal, StopWatchWP(&timeMeasure[1]));
			InterruptHardwareTimerImmitation();
		}
		if (IsTimerWPRinging(&Delay10s)) {
			RestartTimerWP(&Delay10s);
#ifndef MINIMAL_CODESIZE
			printf("10s measured val is: %u\n", StopWatchWP(&timeMeasure[2]));
			//StopTimerWP(&Delay10s);
			UnRegisterTimerCallback(&Timer3s);
#endif // !MINIMAL_CODESIZE
		}
		if (testVarcmd == 1) {
#ifndef MINIMAL_CODESIZE
			UnRegisterTimerCallback(&Timer1s);
#endif // !MINIMAL_CODESIZE
			RestartTimerGroup(SomeProcess.Timers, sizeof(SomeProcess.Timers) / sizeof(Timert_t));
		}
#ifndef MINIMAL_CODESIZE
		if (testVarcmd == 2)
			UnRegisterTimerCallback(&Timer2s);
		if(testVarcmd == 3)
			UnRegisterTimerCallback(&Timer10sOne);
		if (testVarcmd == 4)
			UnRegisterTimerCallback(&Timer15s);
		if(testVarcmd == 5)
			UnRegisterTimerCallback(&Timer5sOneS);
#endif // !MINIMAL_CODESIZE
#ifdef USE_RTOS
		if (testVarcmd == 6) {
			if(!IsTimerWPStarted(&Timer3s))
				TaskYieldWithinSpecifiedTime((U32_ms)15000, &Timer3s); //As example
		}
#endif // USE_RTOS
		if (testVarcmd == 7) {
			InitTimersRegistration();
		}
#ifndef MINIMAL_CODESIZE
		if (testVarcmd == 8) { //Unreg all timers
			Timert_t* timPtr = RegisteredTimers[getRegisterTimersMaxIndex()];
			for (timPtr; timPtr != NULL; timPtr->next) {
				UnRegisterTimerCallback(timPtr);
				timPtr = RegisteredTimers[getRegisterTimersMaxIndex()];
			}
		}
		if (testVarcmd == 9) {
			LoadTooMuchTimers();
			testVarcmd = 0; //Do it only once!
		}
#endif // !MINIMAL_CODE
		if (testVarcmd == 10) {
			StressTests();
		}
		simulateTick();		
	}
}


void InterruptHardwareTimerImmitation(void)
{
#ifndef MINIMAL_CODESIZE
	RegisteredTimersCallbackHandle(RegisteredTimers[getRegisterTimersMaxIndex()]);
#endif // !MINIMAL_CODESIZE
	return;
}

static void TimersCallback(void* arg)
{
	Timert_t* timPtr = arg;
	printf("%u ms left!\n", timPtr->setVal);
	printf("%u ms measured value\n", StopWatchWP(&timeMeasure[0]));
	return;
}

static void AnotherCallback(void* arg)
{
	Timert_t* timPtr = arg;
	if(timPtr->setVal == 10000)
		printf("10s left! HAHAHA!\n");
	else
		printf("%u ms left! It's not 10s!\n", timPtr->setVal);
	return;
}

static void ThirdCallback(void* arg)
{
	uint8_t data = ((uint8_t *)arg)[0];
	uint8_t data2 = *(const uint8_t *)arg;  //same as above
	if (data)
		printf("after 15s, var is TRUE!\n");
	else
		printf("after 15s, var is FALSE!\n");
	if(StopWatchWP(&Test15s))
		printf("Measured time is %u ms!\n", Test15s.measuredTime);  //measuredTime = 15015...15016 ms
}

static uint32_t myTick = 0;
//Some Example:
static void simulateTick(void)
{
	myTick++;
#ifdef MINIMAL_CODESIZE
	someExternalTick++;
#endif // !MINIMAL_CODESIZE
	return;
	/*As example use this func as a reference tick to your specific timer:*/
	/*
	Timert_t MySimulatorTimer = {0};
	MySimulatorTimer.ptrToTick = (tickptr_fn*)getSimulatedTick;*/
}

static uint32_t getSimulatedTick(void)
{
	return myTick;
}

#ifndef MINIMAL_CODESIZE
static Timert_t Timers[MAX_REGISTER_NUM + 3];
static void LoadTooMuchTimers(void)
{
	uint8_t DangerousNum = (MAX_REGISTER_NUM + 3);
	memset(Timers, 0, sizeof(Timers));
	for (uint8_t i = 0; i < DangerousNum; i++) {
		RegisterTimerCallback(&Timers[i], (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	}
	return;
}
#endif // !MINIMAL_CODESIZE

static void StressTests(void)
{
	/*empty yet*/
	return;
}