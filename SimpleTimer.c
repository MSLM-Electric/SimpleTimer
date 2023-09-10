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