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
Timer15s = {0},
ms_Delay = { 0 }, Delay10s = { 0 };
stopwatchwp_t Test15s = { 0 };
uint8_t testVarcmd = 0;
extern Timerwp_t* RegisteredTimers[];
static void TimersCallback(void* arg);
static void AnotherCallback(void* arg);
static void ThirdCallback(void* arg);
void InterruptHardwareTimerImmitation(void);
static void simulateTick(void);
static uint32_t getSimulatedTick(void);

int main(void)
{
	RegisterTimerCallback(&Timer1s, (timerwpcallback_fn *)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer2s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer3s, (timerwpcallback_fn*)TimersCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer5sOneS, (timerwpcallback_fn*)TimersCallback, ONE_SHOT_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer10sOne, (timerwpcallback_fn*)AnotherCallback, ONE_SHOT_TIMER, (tickptr_fn*)GetTickCount);
	RegisterTimerCallback(&Timer15s, (timerwpcallback_fn*)ThirdCallback, PERIODIC_TIMER, (tickptr_fn*)GetTickCount);
	Timer15s.arg = &testVarcmd;
	Test15s.ptrToTick = (tickptr_fn*)GetTickCount;
	LaunchTimerWP(1000, &Timer1s);
	LaunchTimerWP(2000, &Timer2s);
	LaunchTimerWP(3000, &Timer3s);
	LaunchTimerWP(5000, &Timer5sOneS);	
	LaunchTimerWP((U32_ms)10000, &Timer10sOne);
	LaunchTimerWP(15000, &Timer15s);
	InitTimerWP(&ms_Delay, (tickptr_fn*)GetTickCount);  //its just simple timer/delay. It func. is just do some delay operation not precisiously
	InitTimerWP(&Delay10s, (tickptr_fn*)GetTickCount);
	LaunchTimerWP((U32_ms)10, &ms_Delay); //10ms delay
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
		if (testVarcmd == 4)
			UnRegisterTimerCallback(&Timer15s);
		if(testVarcmd == 5)
			UnRegisterTimerCallback(&Timer5sOneS);
		//simulateTick();
	}
}


void InterruptHardwareTimerImmitation(void)
{
	RegisteredTimersCallbackHandle(RegisteredTimers[getRegistersMaxIndex()]);
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
	if(timPtr->setVal == 10000)
		printf("10s left! HAHAHA!\n");
	else
		printf("%u ms left! It's not 10s!\n", timPtr->setVal);
	return;
}

static void ThirdCallback(void* arg)
{
	uint8_t data = ((uint8_t *)arg)[0];
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
	return;
	/*As example use this func as a reference tick to your specific timer:*/
	/*
	Timerwp_t MySimulatorTimer = {0};
	MySimulatorTimer.ptrToTick = (tickptr_fn*)getSimulatedTick;*/
}

static uint32_t getSimulatedTick(void)
{
	return myTick;
}