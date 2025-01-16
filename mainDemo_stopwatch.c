#include <stdint.h>
#include "SimpleTimerWP.h"
#ifdef DEBUG_ON_VS
#include <Windows.h>
#endif // DEBUG_ON_VS
#include <stdio.h>

uint32_t MyTickTime(void);
void IncMyTick(void);
static void Process1(void);
static void Process2(void);
static void Process3(void);

stopwatchwp_t Process1Time = { 0 }, cyclicMeasure = { 0 }, testCyclicMeasure = { 0 }, Process2Time = { 0 };
Timert_t MyTimer = {0};
uint32_t AccumulateVar = 0;
uint8_t catchPoint = 0;
uint32_t justTestVar = 0;


int main(void)
{
#ifdef DEBUG_ON_VS
	InitStopWatchWP(&Process1Time, (tickptr_fn*)GetTickCount);
	InitStopWatchWP(&Process2Time, (tickptr_fn*)GetTickCount);
	InitStopWatchWP(&cyclicMeasure, (tickptr_fn*)GetTickCount);
#endif // DEBUG_ON_VS
	InitTimerWP(&MyTimer, (tickptr_fn*)MyTickTime);
	LaunchTimerWP(15, &MyTimer);

	while (1)
	{
#ifdef DEBUG_ON_VS
		StopWatchWP(&Process1Time);
		Process1();
		StopWatchWP(&Process1Time);

		StopWatchWP(&Process2Time);
		Process2();
		StopWatchWP(&Process2Time);

		uint8_t i = 0;
		for (i; i <= 8; i++) 
		{
			CyclicStopWatchWP(&cyclicMeasure, 8);
			Process3();
			catchBreakPoint(&justTestVar);
		}

		/*lets print all measures to console:*/
		printf("Process1 measure time: %u ms\n", Process1Time.measuredTime);
		printf("Process2 measure time: %u ms\n", Process2Time.measuredTime);
		printf("Process3 cyclicmeasure time: %u ms\n", cyclicMeasure.measuredTime);
#endif // DEBUG_ON_VS

		IncMyTick(); //immitate the tick
	}
}

static uint32_t myTickVal = 0;

void IncMyTick(void)
{
	myTickVal++;
}

uint32_t MyTickTime(void)
{
	return myTickVal;
}

static void Process1(void)
{
#ifdef DEBUG_ON_VS
	Sleep(1000);
#endif // DEBUG_ON_VS
}

static void Process2(void)
{
#ifdef DEBUG_ON_VS
	Sleep(2000);
#endif // DEBUG_ON_VS
}

static void Process3(void)
{
#ifdef DEBUG_ON_VS
	Sleep(100);
#endif // DEBUG_ON_VS
}