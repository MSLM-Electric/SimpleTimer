#ifndef MULTITHREADSUPPORT_H
#define MULTITHREADSUPPORT_H

#define _CRT_SECURE_NO_WARNINGS  //!!! to allow unsafe and oldest code styles

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include <windows.system.threading.h>
//#include <processthreadsapi.h>
#include <Windows.h>
#include <strsafe.h>

#define DEBUG_ON_VS

typedef DWORD* (WINAPI ThreadNamePtr_fn)(LPVOID lpParam);

typedef struct {
	int Data_Of_Thread;
	HANDLE Handle_Of_Thread;
	ThreadNamePtr_fn* ptrToThread;
}ThreadsStruct_t;

int ThreadCreation(ThreadNamePtr_fn* ThreadName, ThreadsStruct_t* ThreadStruct, int Data_Of_Thread);
int ThreadCreationWithPriority(ThreadNamePtr_fn* ThreadName, ThreadsStruct_t* ThreadStruct, int Data_Of_Thread, const int ThreadPriority);
int ThreadInit(LPVOID lpParam);
//int CreateMutex(HANDLE* mutexId, char* nameOfMutex); //InitMutex()
int TakeMutex(HANDLE* mutexId, const uint32_t timeout);
#endif // !MULTITHREADSUPPORT_H
