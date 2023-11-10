/**
* Author          : Osim Abdulhamidov
* Company         : MSLM Electric & Engineering
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
* 3. The name of the author may not be used to endorse or promote products
* derived from this software without specific prior written permission.
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

/*static*/ Timerwp_t* RegisteredTimers[MAX_REGISTER_NUM];
static uint8_t NRegister = 0;

void InitStopWatchWP(stopwatchwp_t* timeMeasure, tickptr_fn* SpecifyTickFunction)
{
	memset(timeMeasure, 0, sizeof(stopwatchwp_t));
	timeMeasure->ptrToTick = SpecifyTickFunction;
}

void InitTimerWP(Timerwp_t* Timer, tickptr_fn* SpecifyTickFunction)
{
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
	if (Timer->ptrToTick == NULL)
		return;
	if (Timer != NULL) {
		if (Timer->Start == 0)
		{
			Timer->setVal = time;
			Timer->launchedTime = (uint32_t)(Timer->ptrToTick());
		}
		Timer->Start = 1;
	}
	return;
}

void StopTimerWP(Timerwp_t* Timer) //or RestartTimer
{
	if (Timer->ptrToTick == NULL)
		return;
	if (Timer != NULL) {
		Timer->setVal = 0;
		Timer->launchedTime = 0;
		Timer->Start = 0;
	}
	return;
}

uint8_t IsTimerWPStarted(Timerwp_t* Timer) {
	if (Timer->ptrToTick == NULL)
		return 0;
	if (Timer != NULL) {
		return Timer->Start;
	}
	return 0;
}

uint8_t IsTimerWPRinging(Timerwp_t* Timer) {
	if (Timer->ptrToTick == NULL)
		return 0;
	if (Timer != NULL) {
        uint32_t tickTime = (uint32_t)(Timer->ptrToTick());
		if (((tickTime - Timer->launchedTime) > Timer->setVal) * Timer->Start)
			return 1; //yes, timer is ringing!
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
uint8_t RegisterTimerCallback(Timerwp_t* Timer, timerwpcallback_fn* ThisTimerCallback, enum timerType_enum timType, tickptr_fn *SpecifyTickFunc)
{
	if (NRegister) {
		if (NRegister < MAX_REGISTER_NUM)
			Timer->next = (Timerwp_t *)RegisteredTimers[NRegister - 1];
		else
			return 240;
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
				else
					RegisteredTimers[n] = NULL;
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

uint8_t getRegistersMaxIndex(void)
{
	if (NRegister > 0) {
		return NRegister - 1;
	}
	return 255; //bad res!
}
#endif // USE_REGISTERING_TIMERS_WITH_CALLBACK
