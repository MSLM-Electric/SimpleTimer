#ifndef __SIMPLETIMER_H_
#define __SIMPLETIMER_H_  //Protection from the headers are getting included multiple time

#include <stdint.h>

#define DEBUG_ON_VS
#ifndef DEBUG_ON_VS
#define HAL_INCTICK
#endif // !DEBUG

typedef struct {
	uint32_t setVal;
	uint32_t launchedTime;
	uint8_t Start;
}Timer_t;

typedef uint32_t U32_ms;
typedef uint32_t U32_us;

void LaunchSpecifiedTimer(uint32_t time, Timer_t* Timer);
void StopSpecifiedTimer(Timer_t* Timer);
uint8_t IsTimerRinging(Timer_t* Timer);
#endif // !__SIMPLETIMER_H_