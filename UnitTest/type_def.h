#ifndef TYPE_DEF_H
#define TYPE_DEF_H
#include "stdint.h"
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

#if defined __MINGW32__
#define MCU_PACK __attribute__((packed, gcc_struct))
#elif defined DEBUG_ON_VS
#define STRINGIFY(a) #a
#define PRAGMA _Pragma
#define MCU_PACK PRAGMA(STRINGIFY(pack(push, 4)))
#define END_MCU_PACK PRAGMA(STRINGIFY(pack(pop)))
#else
#define MCU_PACK __attribute__((packed))
#endif
typedef signed long long int s64;
typedef signed long int s32;
typedef signed short s16;
typedef signed char s8;
typedef signed char i8;

typedef volatile signed long long int vs64;
typedef volatile signed long int vs32;
typedef volatile signed short vs16;
typedef volatile signed char vs8;
typedef volatile signed char vi8;

typedef uint64_t u64;
typedef unsigned long int u32;
typedef unsigned int ui32;//for get rid of warning messege
typedef unsigned short u16;
typedef unsigned char u8;

typedef const unsigned long long int uc64;
typedef const unsigned long int uc32;
typedef const unsigned short uc16;
typedef const unsigned char uc8;

typedef volatile unsigned long int vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char vu8;

typedef volatile unsigned long int const vuc32;
typedef volatile unsigned short const vuc16;
typedef volatile unsigned char const vuc8;

typedef u8 u8_t;
typedef u16 u16_t;
typedef u32 u32_t;

#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif

#ifndef NOT
#define NOT !
#endif // !NOT

#ifndef ONLY
#define ONLY  //!just for beautyfying and readabiling/readability code
#define ENABLED_ONLY ONLY
#endif // !ONLY

#ifndef STILL
#define STILL  //!just for beautyfying and readabiling code
#endif // !STILL

#ifndef NOTHING
#define NOTHING 0 //!just for beautyfying and readability code
#endif // !NOTHING

#ifndef STILL_ONLY_THESE
#define STILL_ONLY_THESE
#endif // !STILL_ONLY_THESE

#ifndef SOMETHING 
#ifdef NULL
#define SOMETHING ~NULL
#endif // NULL
#endif // !SOMETHING 


#ifndef UNUSED
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif // !UNUSED

#define clearBITS(x) &= ~(u32)(x)
#define setBITS(x) |= (x)
//#define BITPOS(x) y = while((x >> 1) > 1){y++};
#define asm __asm
//u16 BitPos(u16 Bit);
//u16 BitPos(u16 Bit)
//{
//	u16 res = 0;
//	while ((Bit >> res) > 1) {
//		res++;
//	}
//	return res;
//}

#ifndef DEBUG_PRINTF
#ifndef DISABLE_LOGS
#define DEBUG_PRINTF(enable, message) do{\
                                     if(enable)\
                                        printf message;\
                                    }while(0)\

#define DEBUG_PRINTFS(enable, message) do{\
                                     if(enable)\
                                        printf_s message;\
                                    }while(0)\

#else
#define DEBUG_PRINTF(enable, message)
#define DEBUG_PRINTFS(enable, message)
#endif // !DISABLE_LOGS
#endif // !DEBUG_PRINTF

#define LINE_EXECUTE_PRINT(x) do{\
                            if(x)\
                                printf("line executed: %s: %d\n", __FILE__, __LINE__); \
                        }while(0)\

/*gcc (optimiz. QT) not compatible/dangerous //? void *CUT_FILES_PATH()
#define MINIMIZE_FILE_PATH
#ifdef MINIMIZE_FILE_PATH
//MINIMIZE_FILE_PATH for unload to printing
inline char* CUT_FILES_PATH(char* x, int siz) {
    if (siz > 3) {
        if (x[siz - 2] == '.')
            if ((x[siz - 1] == 'h') || (x[siz - 1] == 'c')) {
                int pos = siz - 3;
                while ((x[pos--] != '\\') && (pos));
                return (char*)&x[pos];
            }
    }
    return x;
}

#define LINE_EXECUTE_PRINTM(x) do{\
                            if(x)\
                                printf("line executed: %s: %d\n", CUT_FILES_PATH(__FILE__, strlen(__FILE__)), __LINE__); \
                        }while(0)\

#define LINE_EXECUTE_PRINT LINE_EXECUTE_PRINTM
#else
#define LINE_EXECUTE_PRINT LINE_EXECUTE_PRINTF
#endif
#define CUT_TEST LINE_EXECUTE_PRINT*/

//#ifdef __func__
#define FUNCTION_EXECUTE_PRINT(x) do{\
                            if(x)\
                                printf("func executed: %s(): %d\n", __func__, __LINE__); \
                        }while(0)\

//#endif //!__func__

#endif// TYPE_DEF_H
