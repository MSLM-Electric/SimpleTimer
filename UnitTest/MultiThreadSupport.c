#include "MultiThreadSupport.h"
#include "../type_def.h"

int ThreadCreation(ThreadNamePtr_fn* ThreadName, ThreadsStruct_t *ThreadStruct, int Data_Of_Thread)
{
	int res = 0;
	ThreadStruct->Data_Of_Thread = Data_Of_Thread;

	// variable to hold handle of Thread
	ThreadStruct->Handle_Of_Thread = 0;
	ThreadStruct->ptrToThread = ThreadName;

	ThreadStruct->Handle_Of_Thread = CreateThread(NULL, 0, ThreadStruct->ptrToThread, 
		&ThreadStruct->Data_Of_Thread, 0, NULL);
	if (ThreadStruct->Handle_Of_Thread == NULL) {
		res = -1;
		ExitProcess(ThreadStruct->Data_Of_Thread);
	}

	return res;
}

/*This function implemented to not crash the compatibility with old version of project. In future this function may be renamed just to "ThreadCreation"*/
int ThreadCreationWithPriority(ThreadNamePtr_fn* ThreadName, ThreadsStruct_t* ThreadStruct, int Data_Of_Thread, const int ThreadPriority)
{
	int res = 0;
	DWORD /*dwError,*/ dwThreadPri;
	res = ThreadCreation(ThreadName, ThreadStruct, Data_Of_Thread);

	if (!SetThreadPriority(ThreadStruct->Handle_Of_Thread, ThreadPriority))
	{
		DEBUG_PRINTF(1, ("Failed to set thread priority (%d)\n", GetLastError()));
	}
	dwThreadPri = GetThreadPriority(ThreadStruct->Handle_Of_Thread);
	DEBUG_PRINTF(1, ("Priority set: (%d)\n", dwThreadPri));
	return res;
}

int ThreadInit(/*ThreadsStruct_t* ThreadStruct*/LPVOID lpParam)
{
	int res = 0;
	int     Data = 0;
	char a;
	HANDLE  hStdout = NULL;
	// Get Handle To screen. Else how will we print?
	if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return -1;
	// Cast the parameter to the correct
// data type passed by callee i.e main() in our case.
	Data = *((int*)lpParam);
	return res;
}

int TakeMutex(HANDLE *mutexId, const uint32_t timeout)
{
	int res = 0;
	WaitForSingleObject(mutexId, timeout);
	return res;
}