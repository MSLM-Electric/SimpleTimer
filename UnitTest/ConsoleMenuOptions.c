/* version - v0.0.1 */

#include <stdio.h>
#include "ConsoleMenuOptions.h"

#ifdef DEBUG_ON_VS
static void ShowAllStates(void);
#endif // !DEBUG_ON_VS

uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen)
{
	if (inBuff == NULL)
		return -1;
	uint8_t len = maxPossibleLen;
	if (maxPossibleLen == 0) {
		for (len; (len < OPTION_NAMES_LEN) && (inBuff[len] != '\0'); len++);
		//len = OPTION_NAMES_LEN;
	}
	if (len == 0)
		return 0;

	//Users code
	/*---------Put your Functions launch to the header file ConsoleMenuOptions.h----------*/
	/*-------------------------------------------------------------------------------------*/
	u8 OptnQnty = sizeof(MenuOption) / sizeof(menuChoise_t);
	for (uint8_t u = 0; u < OptnQnty; u++) {
		if (strncmp(inBuff, MenuOption[u].optionName, len) == 0)
			return MenuOption[u].optionId;
	}
	return 0;
}

void ConsoleCMDsParse(char* inBuff, const uint16_t maxPossibleLen)
{
	uint8_t parsedCMD = StringCompareAndParseToNum(inBuff, 0);
	ConsolesMenuHandle.CMD[parsedCMD] = ~ConsolesMenuHandle.CMD[parsedCMD] & 0x01;
	uint8_t IsParsedCMD_Enabled = ConsolesMenuHandle.CMD[parsedCMD];
	switch (parsedCMD)
	{
		//Users code
		/*------------------------------Put your Functions launch here----------------------------*/
		//case EXAMPLE: {
		//	ExampleOfYourFunctions();
		//}break;
		/*----------------------------------------------------------------------------------------*/
	case ALL: {
#ifdef DEBUG_ON_VS
		ShowAllStates();
#endif // !DEBUG_ON_VS
	}break;
	case DETAILS: {
		if (IsParsedCMD_Enabled)
			printf("DETAILS ON!\n");
		else
			printf("DETAILS OFF!\n");
	}break;
	case START_COMMUNICATION: {
		if (IsParsedCMD_Enabled) {
			printf("START COMMUNICATION\n");
			ConsolesMenuHandle.CMD[STOP_COMMUNICATION] = 0;
		}
	}break;
	case STOP_COMMUNICATION: {
		if (IsParsedCMD_Enabled) {
			printf("STOP COMMUNICATION\n");
			ConsolesMenuHandle.CMD[START_COMMUNICATION] = 0;
		}
	}break;
	default:
		break;
	}
	//+//?!memset(inBuff, 0, 2/*maxPossibleLen*/); //memsetstr
	return;
}

#ifdef DEBUG_ON_VS
int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen)
{
	int res = -1;
	uint16_t ptr = 0;
	char catchedChar = 0;
	memset(inBuff, 0, maxPossibleLen);
	uint8_t timeout = 200;
	do {
		scanf_s("%s%c", (char*)&inBuff[ptr], maxPossibleLen, &catchedChar, 1);
		ptr = strlen(inBuff);
		if (ptr > maxPossibleLen - 1)
			return -2;
		inBuff[ptr] = ' ';
		ptr++;
	} while ((catchedChar != '\n') && (timeout--)); //(catchedChar == ' ')
	if (timeout == 0)
		return res;
	inBuff[ptr] = '\0';
	return res = 0;
}

#ifdef _CRT_DISABLE_PERFCRIT_LOCKS //!it not unlocks yet
int ScanKeyboardNoLock(char *inBuff, uint16_t maxPossibleLen)
{
	int res = -1;
	int i, ch;

	for (i = 0; (i < maxPossibleLen) && ((ch = _getchar_nolock()) != EOF)
		&& (ch != '\n'); i++)
	{
		inBuff[i] = (char)ch;
	}

	// Terminate string with a null character

	inBuff[i] = '\0';
	printf("Input was: %s\n", inBuff);
}
#endif // _CRT_DISABLE_PERFCRIT_LOCKS
#endif // !DEBUG_ON_VS

void ExecuteCMDsScenarios(char *buffer, const int maxPossibleLen)
{
	u8 u = 0;
	void* arg = NULL; //UNUSED
	for (u = cmdsValEnumsfirstINDEX; u < cmdsValEnumslastINDEX; u++) {
		if ((ConsolesMenuHandle.executeFunc[u] != NULL) && (ConsolesMenuHandle.CMD[u])) {
			ConsolesMenuHandle.executeFunc[u](buffer, maxPossibleLen, arg);
		}
	}
	return;
}

static void ShowAllStates(void)
{
#ifdef DEBUG_ON_VS
#ifdef DEBUG /*|| ENABLE_SOME_STATES_MONITORING*/
	printf("\n\n\n..............\n");
	printf("State 1: = %d\n", State1);
	printf("State 2: = %d\n", State2);
	printf("State 3: = %d\n", State3);
	printf("..............\n");
#define MORE_DETAILS_SHOW 1
#if (MORE_DETAILS_SHOW == 1)
	if (MoreDetailsInShowing) {
		if (State1.Status & OPEN) {
			printf("+--> State1.member: = %d\n", State1.member);
		}
		if (State2.Status & OPEN) {
			printf("+--> State2.member: = %d\n", State2.member);
		}
		if (State3.Status & OPEN) {
			printf("+--> State3 - Status: = %d\n", State3.Status);
			printf("+--> State3 - member: = %d\n", State3.member);
		}
	}
#endif // !MORE_DETAILS_SHOW
#endif // !DEBUG
#endif // !DEBUG_ON_VS
	return;
}