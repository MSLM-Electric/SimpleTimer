#include "SimpleTimer.h"
#ifdef DEBUG_ON_VS
#include <Windows.h>
#elif defined (HAL_INCTICK)
#include "hal.h" //for example "stm32f1xx_hal.h" //or write proper path & name
#elif defined (USER_TICK)
uint32_t someExternalTick;
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
#elif defined(HAL_INCTICK)
			Timer->launchedTime = (uint32_t)HAL_GetTick();
#elif defined (USER_TICK)
			/*Write your code here*/
			Timer->launchedTime = someExternalTick; //as example

			/*  ending your code  */
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
#elif defined(HAL_INCTICK) // DEBUG_ON_VS
		uint32_t tickTime = (uint32_t)HAL_GetTick();
#elif defined (USER_TICK)
		/*Write your code here*/
		uint32_t tickTime = someExternalTick;
		/*  ending your code  */
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
		/*----Remarks----	
		The resolution of the GetTickCount function is limited to the resolution of the system timer, which is typically 
		in the range of 10 milliseconds to 16 milliseconds. The resolution of the GetTickCount function is not affected 
		by adjustments made by the GetSystemTimeAdjustment function.

		The elapsed time is stored as a DWORD value. Therefore, the time will wrap around to zero if the system is run 
		continuously for 49.7 days. To avoid this problem, use the GetTickCount64 function. Otherwise, check for an overflow 
		condition when comparing times.*/
#elif defined(HAL_INCTICK) // DEBUG_ON_VS
		timeWatch = (uint32_t)HAL_GetTick() - timeMeasure->lastTimeFix;
		timeMeasure->lastTimeFix = (uint32_t)HAL_GetTick();
#elif defined (USER_TICK)
		/*Write your code here*/
		timeWatch = someExternalTick - timeMeasure->lastTimeFix;
		timeMeasure->lastTimeFix = someExternalTick;
		/*  ending your code  */
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
		timeMeasure->_tempCycle = timeMeasure->measureCycle + 1;
		if (Ncycle != 0) {
#ifdef DEBUG_ON_VS	
			timeMeasure->lastTimeFix = (uint32_t)GetTickCount();
#elif defined(HAL_INCTICK)
			timeMeasure->lastTimeFix = (uint32_t)HAL_GetTick();
#elif defined (USER_TICK)
			/*Write your code here*/
			timeMeasure->lastTimeFix = someExternalTick;
			/*  ending your code  */
#endif
		}
	}
	if (timeMeasure->_tempCycle > 0)
		timeMeasure->_tempCycle--;
	if (timeMeasure->_tempCycle == 0) {
		return StopWatch(timeMeasure);
	}
	return 0;
}
