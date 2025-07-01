/* version - v0.0.1 */

#ifndef CONSOLEMENUOPTIONS_H
#define CONSOLEMENUOPTIONS_H

#include <stdint.h>
#include "type_def.h"

enum cmdsValEnums {
	cmdsValEnumsfirstINDEX = 1,
	ALL = cmdsValEnumsfirstINDEX,  //it shows all states of your functns
	DETAILS,
	START_COMMUNICATION,
	STOP_COMMUNICATION,
	DEFAULT_CONFIGS,
	RESET_PORT,
	HELP,
	cmdsValEnumslastINDEX
};

#define OPTION_NAMES_LEN 8 //11
typedef struct {
	char optionName[OPTION_NAMES_LEN];
	enum cmdsValEnums optionId;
}menuChoise_t;

static menuChoise_t MenuOption[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX] = {
	{"all\0", ALL},
	{"setfrq-", DETAILS},
	{"start\0", START_COMMUNICATION}, //"STARTCOMMUNIC"
	{"stop\0", STOP_COMMUNICATION},   //"STOPCOMMUNIC"
	{"defcfg\0", DEFAULT_CONFIGS},
	{"rstport", RESET_PORT},
	{"help\0", HELP},
	//Users code
	/*------------------------------Put your Functions launch here----------------------------*/
	//{"example", EXAMPLE},
	/*----------------------------------------------------------------------------------------*/
};

typedef void* (callback_fn)(char* /*buf*/, const int /*maxLen*/, void* /*arg*/);

struct ConsolesMenuHandle_Type {
	uint8_t CMD[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX];  //?!mb sounds better: IsEnabledCMD[] or IsCMDEnabled[]
	callback_fn* executeFunc[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX];
};
struct ConsolesMenuHandle_Type ConsolesMenuHandle;

uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen);
void ConsoleCMDsParse(char* inBuff, const uint16_t maxPossibleLen);
#ifdef DEBUG_ON_VS
int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen);
int ScanKeyboardNoLock(char* inBuff, uint16_t maxPossibleLen);
#endif // !DEBUG_ON_VS
void ExecuteCMDsScenarios(char* buffer, const int maxPossibleLen);
#define PORTABILITY_WITH_OLD_NAME //If you see this attached to some function, so it's for a while/temporarely. After renaming and fixing all corresponded usings the new name of func will be taken.
#define ExecuteConsoleMenuRegisteredCallbacksFunc ExecuteCMDsScenarios PORTABILITY_WITH_OLD_NAME  //ExecuteRegisteredCallbacksFuncOfCMDs
#define SettingsCMD_Handling ConsoleCMDsParse PORTABILITY_WITH_OLD_NAME
#endif // !CONSOLEMENUOPTIONS_H