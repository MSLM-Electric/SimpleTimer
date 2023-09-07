#include "SimpleTimer.h"

/*This is just simple timer without interrupt callback handler.
put:
time - the value that after reaching it IsTimerRinging(&YourTimer) gets true*/
void LaunchSpecifiedTimer(uint32_t time, Timer_t* Timer)
{
	if (Timer->Start == 0)
	{
		Timer->setVal = time;
#ifdef DEBUG //DEBUG_ON_VS
		Timer->launchedTime = (uint32_t)GetTickCount(); //compiler says the GetTickCount64() more safer and hasn't bug!//!
#else HAL_INCTICK
		Timer->launchedTime = (uint32_t)HAL_GetTick();
#endif
	}
	Timer->Start = 1;
}

void StopSpecifiedTimer(Timer_t* Timer) //or RestartTimer
{
	Timer->setVal = 0;
	Timer->launchedTime = 0;
	Timer->Start = 0;
}

uint8_t IsTimerRinging(Timer_t* Timer) {
#ifdef DEBUG
	uint32_t tickTime = (uint32_t)GetTickCount();
#else HAL_INCTICK // DEBUG_ON_VS
	uint32_t tickTime = (uint32_t)HAL_GetTick();
#endif
	if (((tickTime - Timer->launchedTime) > Timer->setVal) * Timer->Start)
		return 1; //yes, timer is ringing!
	return 0; //nope!
}