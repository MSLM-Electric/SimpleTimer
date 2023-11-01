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
Timer10sOne = {0},
ms_Delay = { 0 }, Delay10s = { 0 };
uint8_t testVarcmd = 0;
extern Timerwp_t* RegisteredTimers[];
extern uint8_t NRegister;
static void TimersCallback(void* arg);
static void AnotherCallback(void* arg);
void InterruptHardwareTimerImmitation(void);
static uint32_t simulateTick(void);

int main(void)
{
	RegisterTimerCallback(&Timer1s, (timerwpcallback_fn *)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer2s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer3s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer5sOneS, (timerwpcallback_fn*)TimersCallback, ONE_SHOT_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer10sOne, (timerwpcallback_fn*)AnotherCallback, ONE_SHOT_TIMER, (tickptr_fn*)GetTickCount);
	Timer1s.arg = &Timer1s;
	Timer2s.arg = &Timer2s;
	Timer3s.arg = &Timer3s;
	Timer5sOneS.arg = &Timer5sOneS;
	Timer10sOne.arg = &Timer10sOne;
	LaunchTimerWP(1000, &Timer1s);
	LaunchTimerWP(2000, &Timer2s);
	LaunchTimerWP(3000, &Timer3s);
	LaunchTimerWP(5000, &Timer5sOneS);	
	LaunchTimerWP((U32_ms)10000, &Timer10sOne);
	InitTimerWP(&ms_Delay, (tickptr_fn*)GetTickCount);  //its just simple timer/delay. It func. is just do some delay operation not precisiously
	InitTimerWP(&Delay10s, (tickptr_fn*)GetTickCount);
	LaunchTimerWP((U32_ms)10, &ms_Delay);
	LaunchTimerWP((U32_ms)10000, &Delay10s);
	while (1)
	{		
		if (IsTimerWPRinging(&ms_Delay)) {
			RestartTimerWP(&ms_Delay);
			InterruptHardwareTimerImmitation();
		}
		if (IsTimerWPRinging(&Delay10s)) {
			StopTimerWP(&Delay10s);
			UnRegisterTimerCallback(&Timer3s);
		}
		if(testVarcmd==1)
			UnRegisterTimerCallback(&Timer1s);
		if (testVarcmd == 2)
			UnRegisterTimerCallback(&Timer2s);
		if(testVarcmd == 3)
			UnRegisterTimerCallback(&Timer10sOne);
		//simulateTick();
	}
}


void InterruptHardwareTimerImmitation(void)
{
	RegisteredTimersCallbackHandle(RegisteredTimers[NRegister-1]);
	return;
}

static void TimersCallback(void* arg)
{
	Timerwp_t* timPtr = arg;
	printf("%u ms left!\n", timPtr->setVal);
	return;
}

static void AnotherCallback(void* arg)
{
	Timerwp_t* timPtr = arg;
	printf("%u ms left! HAHAHA!\n", timPtr->setVal);
	return;
}

static uint32_t globTick = 0;
static uint32_t simulateTick(void)
{
	globTick++;
	return globTick;
}