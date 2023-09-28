#include "SimpleTimer.h"
#ifdef DEBUG_ON_VS
#include <Windows.h>
#elif defined (HAL_INCTICK)
#include "hal.h" //or write proper path & name
#endif // DEBUG

/*This is just simple timer without interrupt callback handler and it works without interrupt.
Just use IsTimerRinging() request.
put:
time - the value that after reaching it IsTimerRinging(&YourTimer) gets true*/
void LaunchSpecifiedTimer(uint32_t time, Timer_t* Timer)
{
	if (Timer != NULL) {
		if (Timer->Start == 0)
		{
			Timer->setVal = time;
#ifdef DEBUG_ON_VS //DEBUG_ON_VS
			Timer->launchedTime = (uint32_t)GetTickCount(); //compiler says the GetTickCount64() more safer and hasn't bug!//!
#else HAL_INCTICK
			Timer->launchedTime = (uint32_t)HAL_GetTick();
#endif
		}
		Timer->Start = 1;
	}
	return;
}

void StopSpecifiedTimer(Timer_t* Timer) //or RestartTimer
{
	if (Timer != NULL) {
		Timer->setVal = 0;
		Timer->launchedTime = 0;
		Timer->Start = 0;
	}
	return;
}

uint8_t IsTimerStarted(Timer_t* Timer) {
	if (Timer != NULL) {
		return Timer->Start;
	}
	return 0;
}

uint8_t IsTimerRinging(Timer_t* Timer) {
	if (Timer != NULL) {
#ifdef DEBUG_ON_VS
		uint32_t tickTime = (uint32_t)GetTickCount();
#else HAL_INCTICK // DEBUG_ON_VS
		uint32_t tickTime = (uint32_t)HAL_GetTick();
#endif
		if (((tickTime - Timer->launchedTime) > Timer->setVal) * Timer->Start)
			return 1; //yes, timer is ringing!
	}
	return 0; //nope!
}

/*
Example 1: ... //Measuring one fully programm/algorithm cycle
MeasuredTime = StopWatch(&stopWatch_UDPrecv); //per cycle
Example 2: ... //Measuring point to point
StopWatch(&stopWatch_TimingsBeetwenProcess1and2);                  //1-st point
... ... ...
MeasuredTime = StopWatch(&stopWatch_TimingsBeetwenProcess1and2);   //2-nd point
*/
uint32_t StopWatch(stopwatch_t *timeMeasure/*, uint8_t measureType*/)
{
	//timeMeasure->measureType = measureType;
	uint32_t timeWatch = 0;
	uint32_t firstTimeLaunch = timeMeasure->lastTimeFix;
#ifdef DEBUG_ON_VS	
		timeWatch = (uint32_t)GetTickCount() - timeMeasure->lastTimeFix;
		timeMeasure->lastTimeFix = (uint32_t)GetTickCount();
#else HAL_INCTICK // DEBUG_ON_VS
		timeWatch = (uint32_t)HAL_GetTick() - timeMeasure->lastTimeFix;
		timeMeasure->lastTimeFix = (uint32_t)HAL_GetTick();
#endif
	if (firstTimeLaunch == 0)
		return 0;
	timeMeasure->measuredTime = timeWatch;
	return timeWatch;
}


uint32_t CyclicStopWatch(stopwatch_t* timeMeasure, uint16_t Ncycle)
{
	if (timeMeasure->_tempCycle == 0) {
		timeMeasure->measureCycle = Ncycle;
		timeMeasure->_tempCycle = timeMeasure->measureCycle+1;
		if (Ncycle != 0)
			timeMeasure->lastTimeFix = (uint32_t)GetTickCount();
	}
	if (timeMeasure->_tempCycle > 0)
		timeMeasure->_tempCycle--;
	if (timeMeasure->_tempCycle == 0) {
		return StopWatch(timeMeasure);
	}
	return 0;
}