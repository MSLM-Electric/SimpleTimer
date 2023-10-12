#include <stdint.h>
#include "SimpleTimer.h"
#ifdef DEBUG_ON_VS
#include <Windows.h>
#endif // DEBUG_ON_VS

stopwatch_t measureTime = { 0 }, cyclicMeasure = { 0 }, testCyclicMeasure = { 0 }, simpleMeasure = { 0 };
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
	}
}
