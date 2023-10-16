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

#include "SimpleTimerWP.h"  //WP means "WITH POINTER" i.e Simple Timer with pointer. 
/*you can set to it pointer the address of your project TickValue getting function.
For example: -->
Timerwp_t MyTimer1;
Timerwp_t MyTimer2;
stopwatchwp_t microsecondT;
MyTimer1.ptrToTick = (tickptr_fn*)GetTickCount;
MyTimer2.ptrToTick = (tickptr_fn*)HAL_GetTick;
microsecondT.ptrToTick = (tickptr_fn*)usTick;
-->
And the main functions of this library will fucntioning
refering to the fucntions mentioned on above.

uint32_t usTick(void)
{
	return usIncTickValue;
}

or just:
InitStopWatchWP(&microsecondT, (tickptr_fn*)usTick);
InitTimerWP(&MyTimer1, (tickptr_fn*)HAL_GetTick); //for MyTimer1 use the HAL_GetTick() fucntion;
*/
void InitStopWatchWP(stopwatchwp_t* timeMeasure, tickptr_fn* SpecifyTickFunction)
{
	memset(timeMeasure, 0, sizeof(timeMeasure));
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
			timeMeasure->lastTimeFix = (uint32_t)timeMeasure->ptrToTick();
		}
	}
	if (timeMeasure->_tempCycle > 0)
		timeMeasure->_tempCycle--;
	if (timeMeasure->_tempCycle == 0) {
		return StopWatchWP(timeMeasure);
	}
	return 0;
}

void LaunchTimerWP(uint32_t time, Timerwp_t* Timer)
{
	if (Timer->ptrToTick == NULL)
		return 0;
	if (Timer != NULL) {
		if (Timer->Start == 0)
		{
			Timer->setVal = time;
			Timer->launchedTime = (uint32_t)Timer->ptrToTick(); 
		}
		Timer->Start = 1;
	}
	return;
}

void StopTimerWP(Timerwp_t* Timer) //or RestartTimer
{
	if (Timer->ptrToTick == NULL)
		return 0;
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
		uint32_t tickTime = (uint32_t)Timer->ptrToTick();
		if (((tickTime - Timer->launchedTime) > Timer->setVal) * Timer->Start)
			return 1; //yes, timer is ringing!
	}
	return 0; //nope!
}