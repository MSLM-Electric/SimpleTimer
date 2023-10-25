#include <stdint.h>
#include "SimpleTimerWP.h"
#ifdef DEBUG_ON_VS
#include <stdio.h>
#include <Windows.h>
#endif // DEBUG_ON_VS

Timerwp_t Timer3s = { 0 },
Timer2s = { 0 }, 
Timer1s = { 0 }, 
Timer5sOneS = { 0 },
ms_Delay = { 0 };
extern Timerwp_t* RegisteredTimers[];
extern uint8_t NRegister;
static void TimersCallback(void* arg);
void InterruptHardwareTimerImmit(void);
static uint32_t simulateTick(void);

int main(void)
{
	RegisterTimerCallback(&Timer1s, (timerwpcallback_fn *)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer2s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer3s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer5sOneS, (timerwpcallback_fn*)TimersCallback, ONE_SHOT_TIMER, (tickptr_fn*)GetTickCount);
	Timer1s.arg = &Timer1s;
	Timer2s.arg = &Timer2s;
	Timer3s.arg = &Timer3s;
	Timer5sOneS.arg = &Timer5sOneS;
	LaunchTimerWP(1000, &Timer1s);
	LaunchTimerWP(2000, &Timer2s);
	LaunchTimerWP(3000, &Timer3s);
	LaunchTimerWP(5000, &Timer5sOneS);	
	InitTimerWP(&ms_Delay, (tickptr_fn*)GetTickCount);  //its just simple timer/delay. It func. is just do some delay operation not precisiously
	LaunchTimerWP((U32_ms)10, &ms_Delay);
	while (1)
	{		
		if (IsTimerWPRinging(&ms_Delay)) {
			RestartTimerWP(&ms_Delay);
			InterruptHardwareTimerImmit();
		}
		//simulateTick();
	}
}


void InterruptHardwareTimerImmit(void)
{
	IsTimerWPRinging_CallFromISR(RegisteredTimers[NRegister-1]);
	return;
}

static void TimersCallback(void* arg)
{
	Timerwp_t* timPtr = arg;
	printf("%u ms left!\n", timPtr->setVal);
	return;
}

static uint32_t globTick = 0;
static uint32_t simulateTick(void)
{
	globTick++;
	return globTick;
}