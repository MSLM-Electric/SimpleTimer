#define _CRT_SECURE_NO_WARNINGS  //!!! to allow unsafe and oldest code styles

#include "../MultiThreadSupport.h"
#include "../../SimpleTimerWP.h"
#include "../ConsoleMenuOptions.h"

#define false 0
#define true 1

uint8_t someData[128] = {0};
uint8_t getData[1024];
uint16_t catchPoint = 0;

enum {
	INTERRUPT_CALLED = 1,
	NOT_INTERRUPT_STATE = 2
}State_t;

enum {
	DMA_READY = 1,
	DMA_BUSY = 2
}dmastate_t;

typedef struct{
	uint8_t State;
}interrupt_simulate_handle_t, dma_simulate_handle_t;

interrupt_simulate_handle_t callInterrupt;
dma_simulate_handle_t DMAHandle;
Timert_t UsersTimer;
uint8_t ledstate = 0;


DWORD WINAPI ThreadNo1(LPVOID lpParam);
DWORD WINAPI TickThread(LPVOID lpParam);
ThreadsStruct_t Thread1Struct;
ThreadsStruct_t TickThreadStruct;

void callback()
{
	//Users code here
	return;
}

static void ShowAllStates(void);
HANDLE sem;
HANDLE mutx;
char keyboardBuff[20];
uint8_t scanfIsBusy = 0;
uint8_t simulate100msTickVal = 0;

int main()
{
	sem = CreateSemaphoreW(NULL, 3, 3, "NT5BBSEM");
	mutx = CreateMutexW(NULL, 1, "Mutex");

	Sleep(500);
	int res = 0;
	res = ThreadCreationWithPriority(&ThreadNo1, &Thread1Struct, 1, THREAD_PRIORITY_HIGHEST);
	res = ThreadCreationWithPriority(&TickThread, &TickThreadStruct, 2, THREAD_PRIORITY_NORMAL);

	// Aray to store thread handles 
	HANDLE Array_Of_Thread_Handles[2];
	// Store Thread handles in Array of Thread
	// Handles as per the requirement
	// of WaitForMultipleObjects() 
	Array_Of_Thread_Handles[0] = Thread1Struct.Handle_Of_Thread;
	Array_Of_Thread_Handles[1] = TickThreadStruct.Handle_Of_Thread;

	// Wait until all threads have terminated.
	WaitForMultipleObjects(2, Array_Of_Thread_Handles, TRUE, INFINITE);

	memset(someData, 0, sizeof(someData));
	ReleaseMutex(mutx);  //free mutex to start program

	while (1)
	{
		Sleep(1000);
	}
	printf("endOfCycle. Bad jump! \n"); //programm execution never should get here!
}

DWORD WINAPI ThreadNo1(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	//char *keyboardBuff = (char *)malloc(20 * sizeof(char));
	char keyboardBuff[255];
	while (1)
	{
		Sleep(100);
		simulate100msTickVal++;
	}
}

static uint8_t get100msTickVal(void)
{
	return simulate100msTickVal;
}

Timert_t Timer3s;
Timert_t Timer1s;
uint8_t simulate1sTickVal = 0;

#include "../EqualCheck.h"
stopwatchwp_t test3s;

DWORD WINAPI TickThread(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint16_t testCount = 0;
	TimerBaseType TickVal = (TimerBaseType)get100msTickVal();
	LaunchTimerByRef(10 x100ms, &Timer1s, TickVal);
	LaunchTimerByRef(3 x1s, &Timer3s, TickVal);
	InitStopWatchWP(&test3s, (tickptr_fn*)GetTickCount);
	StopWatchWP(&test3s);
	while (1)
	{
		TimerBaseType TickToRef = (TimerBaseType)get100msTickVal();

		if (IsTimerRingingKnowByRef(&Timer1s, TickToRef)) {
			RestartTimerByRef(&Timer1s, TickToRef);
			simulate1sTickVal++;
			if (IsTimerRingingKnowByRef(&Timer3s, simulate1sTickVal)) {
				RestartTimerByRef(&Timer3s, simulate1sTickVal);
				StopWatchWP(&test3s);
				debug_assert("Testing 3s is bad!\n", IsValuesEqual2(test3s.measuredTime, 2997 x1ms, 3006 x1ms), FALSE);
				DEBUG_PRINTF(1, ("Measured 3s testing is: %u ms!\n", test3s.measuredTime));
				DEBUG_PRINTF(1, ("Tick Val = %u\n", simulate1sTickVal));
				if (ledstate) {
					DEBUG_PRINTF(1, ("Ledstate: OFF\n"));
					ledstate = 0;
				}
				else {
					DEBUG_PRINTF(1, ("Ledstate: ON\n"));
					ledstate = 1;
				}
			}
		}
	}
}
