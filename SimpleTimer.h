#include <stdint.h>

#define DEBUG
#ifndef DEBUG
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