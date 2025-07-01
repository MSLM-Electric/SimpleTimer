#define _CRT_SECURE_NO_WARNINGS  //!!! to allow unsafe and oldest code styles

#include "../MultiThreadSupport.h"
#include "../SimpleTimerWP.h"
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
DWORD WINAPI ThreadNo2(LPVOID lpParam);
DWORD WINAPI TickThread(LPVOID lpParam);
ThreadsStruct_t Thread1Struct;
ThreadsStruct_t Thread2Struct;
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

int main()
{
	sem = CreateSemaphoreW(NULL, 3, 3, "NT5BBSEM");
	mutx = CreateMutexW(NULL, 1, "Mutex");

	Sleep(500);
	int res = 0;
	res = ThreadCreationWithPriority(&ThreadNo1, &Thread1Struct, 1, THREAD_PRIORITY_NORMAL);
	res = ThreadCreationWithPriority(&ThreadNo2, &Thread2Struct, 2, THREAD_PRIORITY_NORMAL);
	res = ThreadCreationWithPriority(&TickThread, &TickThreadStruct, 4, THREAD_PRIORITY_HIGHEST);

	// Aray to store thread handles 
	HANDLE Array_Of_Thread_Handles[4];
	// Store Thread handles in Array of Thread
	// Handles as per the requirement
	// of WaitForMultipleObjects() 
	Array_Of_Thread_Handles[0] = Thread1Struct.Handle_Of_Thread;
	Array_Of_Thread_Handles[1] = Thread2Struct.Handle_Of_Thread;
	Array_Of_Thread_Handles[3] = TickThreadStruct.Handle_Of_Thread;

	// Wait until all threads have terminated.
	WaitForMultipleObjects(3, Array_Of_Thread_Handles, TRUE, INFINITE);

	memset(someData, 0, sizeof(someData));
	ReleaseMutex(mutx);  //free mutex to start program
	Timert_t MainProgrammDelay;
#ifdef DEBUG_ON_VS
	InitTimerWP(&UsersTimer, NULL);
	InitTimerWP(&MainProgrammDelay, NULL);
#endif // DEBUG_ON_VS
	TimerBaseType TickVal = GetTickCount();
	LaunchTimerByRef(2000 x1ms, &MainProgrammDelay, TickVal);

	while (1)
	{
		//TickVal = GetTickCount();
		if (IsTimerRingingKnowByRef(&MainProgrammDelay, TickVal))//Sleep(2000);
		{
			RestartTimerByRef(&MainProgrammDelay, TickVal);
			printf("MainBckgdProccess\n");
		}
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
		WaitForSingleObject(mutx, INFINITE);
		{
			memset(keyboardBuff, 0, sizeof(keyboardBuff));
			printf("What function to Act? Enter it here:\n");
			scanf_s("%s", keyboardBuff, 255);
			printf("entered data is: %s\n", keyboardBuff);
		}
		ReleaseMutex(mutx);
		if (keyboardBuff == NULL)
		{
			printf("Memory for str alloc ERROR\t\n");
		}
		SettingsCMD_Handling(keyboardBuff, NULL);
		WaitForSingleObject(mutx, INFINITE);
		ExecuteCMDsScenarios(keyboardBuff, sizeof(keyboardBuff));
		ReleaseMutex(mutx);
	}
}


DWORD WINAPI ThreadNo2(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint8_t buttonForCallInterruptStateChange = 2;
	while (1)
	{
		//Sleep(10);
		WaitForSingleObject(mutx, INFINITE);
		{
			printf_s("Enter The interrupt calling state:\n");
			scanf_s("%d", &buttonForCallInterruptStateChange);
			printf("entered data is: %d\n", buttonForCallInterruptStateChange);
		}
		ReleaseMutex(mutx);
		if (buttonForCallInterruptStateChange == 1) {
			callInterrupt.State = INTERRUPT_CALLED;
			callback();
			callInterrupt.State = NOT_INTERRUPT_STATE;
		}
		if (buttonForCallInterruptStateChange == 2)
			callInterrupt.State = NOT_INTERRUPT_STATE;
		buttonForCallInterruptStateChange = 0;
	}
}

Timert_t Timer1s;
Timert_t Timer10ms;

#include "EqualCheck.h"
stopwatchwp_t test1s;

DWORD WINAPI TickThread(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint16_t testCount = 0;
	TimerBaseType TickVal = (TimerBaseType)GetTickCount();
	LaunchTimerByRef(10 x1ms, &Timer10ms, TickVal);
	LaunchTimerByRef(1000 x1ms, &Timer1s, TickVal);
	InitStopWatchWP(&test1s, (tickptr_fn*)GetTickCount);
	StopWatchWP(&test1s);
	while (1)
	{
		TimerBaseType TickToRef = (TimerBaseType)GetTickCount();

		if (IsTimerRingingKnowByRef(&Timer10ms, TickToRef)) {
			RestartTimerByRef(&Timer10ms, TickToRef);
			if (IsTimerRingingKnowByRef(&Timer1s, TickToRef)) {
				RestartTimerByRef(&Timer1s, TickToRef);
				StopWatchWP(&test1s);
				debug_assert("Testing 1s is bad!\n", IsValuesEqual2(test1s.measuredTime, 997 x1ms, 1006 x1ms), FALSE);
				DEBUG_PRINTF(1, ("Tick Val = %u\n", TickToRef));
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
