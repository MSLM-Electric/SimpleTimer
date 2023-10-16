#include <stdint.h>
#include "SimpleTimer.h"
#ifdef DEBUG_ON_VS
#include <Windows.h>
#endif // DEBUG_ON_VS

uint32_t MyTickTime(void);
void IncMyTick(void);

stopwatch_t measureTime = { 0 }, cyclicMeasure = { 0 }, testCyclicMeasure = { 0 }, simpleMeasure = { 0 };
specStopwatch_t mySpecTimer = { 0 }, mySpecTimer2 = { 0 };
SpecTimer_t Timer1;
uint32_t AccumulateVar = 0;
uint8_t catchPoint = 0;
uint32_t justTestVar = 0;


int main(void)
{
	//;
	while (1)
	{
#ifdef DEBUG_ON_VS
		Sleep(1000);
		mySpecTimer.ptrToTick = (tickptr_fn *)GetTickCount;
		StopWatchSpec(&mySpecTimer);
#endif // DEBUG_ON_VS		
		StopWatch(&simpleMeasure);
		CyclicStopWatch(&measureTime, 0);
		AccumulateVar += measureTime.measuredTime;
		if (CyclicStopWatch(&cyclicMeasure, 3)) {  //when it true, cyclicMeasure.measureTime = 3000ms
			StopWatch(&testCyclicMeasure);
			catchPoint = 2;
			AccumulateVar = 0;
			StopWatch(&testCyclicMeasure);
		}
		justTestVar = ms_x100us((U32_ms)50); //50ms count per 100us  = 500
		justTestVar = ms_x10us((U32_ms)50);  //50ms count per 10us   = 5000

		mySpecTimer2.ptrToTick = (tickptr_fn*)MyTickTime;
		StopWatchSpec(&mySpecTimer2);
		IncMyTick();
		Timer1.ptrToTick = (tickptr_fn*)MyTickTime;
		LaunchTimerSpec(12, &Timer1);
		if (IsTimerSpecRinging(&Timer1)) {
			StopTimerSpec(&Timer1);
			LaunchTimerSpec(15, &Timer1);
		}
#include "SimpleTimerWP.h"
		stopwatchwp_t timeM;
		InitStopWatchWP(&timeM, (tickptr_fn*)GetTickCount);
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