/**
* Author          : Osim Abdulhamidov
* Company         : MSLM Electric
* Project         : Simple timer
* MCU or CPU      : Any
* Created on October 13, 2023
*/
/*
* Copyright(c) 2023 MSLM Electric
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met :
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditionsand the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditionsand the following disclaimer in the documentation
* and /or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT
* SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
	* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	* CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
	* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
	* OF SUCH DAMAGE.

	Author: github.com/MSLM-Electric/
	E-mail: mslmelectric@gmail.com
*/

#include "SimpleTimerWP.h"  //WP means "WITH POINTER". 
/*you can set to it pointer the address of your project TickValue getting function.
For example: -->
Timerwp_t MyTimer1;
Timerwp_t MyTimer2;
stopwatchwp_t microsecondT;
MyTimer1.ptrToTick = (tickptr_fn*)GetTickCount;
MyTimer2.ptrToTick = (tickptr_fn*)HAL_GetTick;
microsecondT.ptrToTick = (tickptr_fn*)usTick;
-->
And the main functions of this library will fucntionate
refering to the fucntions mentioned above.

uint32_t usTick(void)
{
	return usIncTickValue;
}

or just:
InitStopWatchWP(&microsecondT, (tickptr_fn*)usTick);
InitTimerWP(&MyTimer1, (tickptr_fn*)HAL_GetTick); //for MyTimer1 use the HAL_GetTick() fucntion;
*/
#ifdef USE_REGISTERING_TIMERS_WITH_CALLBACK
/*static*/ Timerwp_t* RegisteredTimers[MAX_REGISTER_NUM];
static uint8_t NRegister = 0;
#endif

void InitStopWatchWP(stopwatchwp_t* timeMeasure, tickptr_fn* SpecifyTickFunction)
{
	memset(timeMeasure, 0, sizeof(stopwatchwp_t));
	timeMeasure->ptrToTick = SpecifyTickFunction;
}

void InitTimerWP(Timerwp_t* Timer, tickptr_fn* SpecifyTickFunction)
{
	memset(Timer, 0, sizeof(Timerwp_t));
	Timer->ptrToTick = SpecifyTickFunction;
	StopTimerWP(Timer);
}

//StopWatchPointToPointStart(&watch);
//...
//StopWatchPointToPointEnd(&watch);

uint32_t StopWatchWP(stopwatchwp_t* timeMeasure)
{
	if (timeMeasure->ptrToTick == NULL)
		return 0;
	uint32_t timeWatch = 0;
	uint32_t firstTimeLaunch = timeMeasure->lastTimeFix;
	timeWatch = (uint32_t)(timeMeasure->ptrToTick()) - timeMeasure->lastTimeFix;
	timeMeasure->lastTimeFix = (uint32_t)(timeMeasure->ptrToTick());
	if (firstTimeLaunch == 0)
		return 0;
	timeMeasure->measuredTime = timeWatch;
	return timeWatch;
}

uint32_t CyclicStopWatchWP(stopwatchwp_t* timeMeasure, uint16_t Ncycle)
{
	if (timeMeasure->ptrToTick == NULL)
		return 0; //-1
	if (timeMeasure->_tempCycle == 0) {
		timeMeasure->measureCycle = Ncycle;
		timeMeasure->_tempCycle = timeMeasure->measureCycle + 1;
		if (Ncycle != 0) {
			timeMeasure->lastTimeFix = (uint32_t)(timeMeasure->ptrToTick());
		}
	}
	if (timeMeasure->_tempCycle > 0)
		timeMeasure->_tempCycle--;
	if (timeMeasure->_tempCycle == 0) {
		return StopWatchWP(timeMeasure);
	}
	return 0;
}

/*This is just simple timer without interrupt callback handler and it works without interrupt.
Just use IsTimerWPRinging() request. But before you have to set the .ptrToTick to the tick function reference.
To do it call the InitTimerWP(&MyTimer, (tickptr_fn *)xTickGetCountForExample);
put:
time - the value that after reaching it IsTimerWPRinging(&MyTimer) gets true*/
void LaunchTimerWP(uint32_t time, Timerwp_t* Timer)
{
	if (Timer != NULL) {
		if (Timer->ptrToTick == NULL)
			return;
		if (Timer->Start == 0)
		{
			Timer->setVal = time;
			Timer->launchedTime = (uint32_t)(Timer->ptrToTick());
		}
		Timer->Start = 1;
	}
	return;
}

void StopTimerWP(Timerwp_t* Timer)
{
	if (Timer != NULL) {
		//if (Timer->ptrToTick == NULL)
		//	return;
		//Timer->setVal = 0;
		Timer->launchedTime = 0;
		Timer->Start = 0;
	}
	return;
}

void InitTimerGroup(Timerwp_t* ArrTimers, tickptr_fn* SpecifyTickFunction, uint8_t qntyTimers, uint32_t setVals)
{
	for (uint8_t u = 0; u < qntyTimers; u++)
	{
		InitTimerWP(&ArrTimers[u], SpecifyTickFunction);
		ArrTimers[u].setVal = setVals;
	}
}

void StopTimerGroup(Timerwp_t* ArrTimers, uint8_t qntyTimers)
{
	for (uint8_t u = 0; u < qntyTimers; u++)
	{
		StopTimerWP(&ArrTimers[u]);
	}
}

uint8_t RestartTimerGroup(Timerwp_t* ArrTimers, uint8_t qntyTimers)
{
	uint8_t res = 0;
	for (uint8_t u = 0; u < qntyTimers; u++)
	{
		//if (ArrTimers[u].TimType != PERIODIC_TIMER)
			res |= RestartTimerWP(&ArrTimers[u]);
	}
	return res;
}

uint8_t IsTimerWPStarted(Timerwp_t* Timer) {
	if (Timer != NULL) {
		if (Timer->ptrToTick == NULL)
			return 0;
		return Timer->Start;
	}
	return 0;
}

uint8_t IsTimerWPRinging(Timerwp_t* Timer) {
	if (Timer != NULL) {
		if (Timer->ptrToTick == NULL)
			return 0;
		if (Timer->Start) {
			uint32_t tickTime = (uint32_t)(Timer->ptrToTick());
			if ((tickTime - Timer->launchedTime) > Timer->setVal)
				return 1; //yes, timer is ringing!
		}
	}
	return 0; //nope!
}

uint8_t RestartTimerWP(Timerwp_t* Timer)
{
	if ((Timer->ptrToTick == NULL) || (Timer == NULL))
		return 255;
	if (Timer->setVal < 0)
		return 254;
	Timer->launchedTime = (uint32_t)(Timer->ptrToTick());
	Timer->Start = 1;
	return 0;
}

#ifdef USE_REGISTERING_TIMERS_WITH_CALLBACK
uint8_t RegisterTimerCallback(Timerwp_t* Timer, timerwpcallback_fn* ThisTimerCallback, timerType_enum timType, tickptr_fn *SpecifyTickFunc)
{
	if (NRegister) {
		if (NRegister > MAX_REGISTER_NUM-1)
			return 240;
		for (uint8_t k = 0; k <= NRegister; k++) {  //Check timers existance  //Double registering protection
			if ((Timer == RegisteredTimers[k]))
				return 242; //This timer already registered!
		}		
		Timer->next = (Timerwp_t *)RegisteredTimers[NRegister - 1];
	}
	Timer->RegisteredCallback = ThisTimerCallback;
	RegisteredTimers[NRegister] = Timer;
	NRegister++;
	Timer->TimType = timType;
	Timer->ptrToTick = SpecifyTickFunc;
	if (Timer->arg == NULL)
		Timer->arg = Timer;
	return 0;
}

//state: Tested!
uint8_t UnRegisterTimerCallback(Timerwp_t* Timer)
{
	uint8_t k = 0;
	for (uint8_t i = 0; i < NRegister; i++) {
		if (Timer == RegisteredTimers[i]) {
			Timer->RegisteredCallback = NULL;
			Timer->next = NULL;
			StopTimerWP(Timer);
			RegisteredTimers[i] = NULL;
			k = NRegister; //looks unreadable, but is only for ...
			uint8_t doOnce = 1;
			for (uint8_t n = i + 1; n < NRegister; n++) {
				RegisteredTimers[n - 1] = RegisteredTimers[n];
				if (doOnce && (i > 0)) {
					RegisteredTimers[n]->next = RegisteredTimers[i - 1];
					doOnce = 0;}
				if (i == 0)
					RegisteredTimers[i]->next = NULL;  //delete the nextptr on 0-th array or 1st member on regtimer
			}
			NRegister--;
			break;
		}else
			k++;
	}
	if (k <= NRegister)
		return 241; //This Timer not registered, not found!
	for (uint8_t n = NRegister; n < MAX_REGISTER_NUM; n++) {
		RegisteredTimers[n] = NULL;
	}
	return 0;
}

uint8_t RegisteredTimersCallbackHandle(Timerwp_t* Timer)
{
	if (Timer != NULL) {
		if (Timer->RegisteredCallback != NULL)
		{
			for (Timerwp_t* timPtr = Timer; timPtr != NULL; timPtr = timPtr->next) {
				if (IsTimerWPRinging(timPtr)) {
					timPtr->RegisteredCallback(timPtr->arg);
					if (timPtr->TimType == PERIODIC_TIMER)
						RestartTimerWP(timPtr);
					else {
						//StopTimerWP(Timer);
						timPtr->Start = 0;
					}
				}
			}
		}
	}
	return 0;
}

uint8_t getRegisterTimersMaxIndex(void)
{
	if (NRegister > 0) {
		return NRegister - 1;
	}
	return 0; //255 bad res! //maybe it has sense to return 0 even when error; cause the protections on functions already prepared! (If don't to do it that mayb very dangerous code while using RegisteredTimersCallbackHandle(RegisteredTimers[255])!)
}
#endif // USE_REGISTERING_TIMERS_WITH_CALLBACK

#ifdef USING_RTOS
#ifndef taskYIELD
//#include "cmsis_os.h" //as Example. //put here the Task switch context macro from your RTOS or include the RTOS header
#ifdef DEBUG_ON_VS
#define taskYIELD()
#endif // DEBUG_ON_VS
#endif // !taskYIELD
void TaskYieldWithinSpecifiedTime(const uint32_t time, Timerwp_t* Timer)
{
	LaunchTimerWP(time, Timer);
	while (!IsTimerWPRinging(Timer))
	{
		taskYIELD();
	}
	StopTimerWP(Timer);
}
#endif // USING_RTOS

void catchBreakPoint(uint32_t* var)
{
	*var = *var + 1;
	*var = *var - 1;
	return;
}