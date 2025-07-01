#ifndef TYPE_DEF_H
#define TYPE_DEF_H
#include <stdint.h>
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

#include <stdio.h>

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
typedef s8 s8_t;
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
#endif // !ONLY

#ifndef STILL
#define STILL  //!just for beautyfying and readabiling code
#endif // !STILL

#ifndef NOTHING
#define NOTHING 0 //!just for beautyfying and readability code
#ifndef NONE
#define NONE NOTHING
#endif // !NONE
#endif // !NOTHING

#ifndef SOMETHING 
#ifdef NULL
#define SOMETHING ~NULL
#endif // NULL
#endif // !SOMETHING 


#ifndef UNUSED
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif // !UNUSED

#define clearBIT(x) &= ~(u32)(x)
#define setBIT(x) |= (x)
#ifndef BIT
#define BIT(x)  (1 << x)
#endif

#define ARRAY_SIZE_BY_TYPE(arrptr, type) sizeof(arrptr)/sizeof(type)

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

#define FUNCTION_EXECUTE_PRINT_EN 1
#if (FUNCTION_EXECUTE_PRINT_EN )//&& defined (__func__)) //or __FUNCTION__
#define FUNCTION_EXECUTE_PRINT(x) do{\
                            if(x)\
                                printf("func executed: %s(): %d\n", __func__, __LINE__); \
                        }while(0)\
/*//?not FreeRTOS debug_print.h instead printf()?*/
#else
#define FUNCTION_EXECUTE_PRINT(x)
#endif // !(FUNCTION_EXECUTE_PRINT_EN )

#define FAILED_EXECUTE_MSG_EN 1
#if (FAILED_EXECUTE_MSG_EN)
#define FAILED_EXECUTE_MSG(x) do{\
                            if(x)\
                                printf("Failed execute on: %s(): %d\n", __func__, __LINE__); \
                        }while(0)\

#else
#define FAILED_EXECUTE_MSG(x)
#endif // !FAILED_EXECUTE_MSG_EN

#define asm __asm

//#if defined(_MSC_VER)
//#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
//#else
//#if defined(__GNUC__)
//#define DLL_PUBLIC __attribute__ ((dllexport))
//#endif
//#endif
//
#define MCU_ROOT_CODE //__attribute__((section(".priv_code")))
#define MCU_PRIV_CODE MCU_ROOT_CODE
//#define TASK_CODE __attribute__ ((section (".task_text")))
//#define INLINE __attribute__((always_inline)) __STATIC_INLINE
//
//
//#define asm __asm
//

#define DEBUG_ASSERT_EN 1

#if defined __MINGW32__
#define MCU_PACK __attribute__((packed, gcc_struct))
#define END_MCU_PACK
#elif defined DEBUG_ON_VS
#define STRINGIFY(a) #a
#define PRAGMA _Pragma
#define MCU_PACK //PRAGMA(STRINGIFY(pack(push, 4)))
#define END_MCU_PACK PRAGMA(STRINGIFY(pack(pop)))
#define MCU_ALIGN(x)
#define debug_printf printf
#if (DEBUG_ASSERT_EN==1)
#define debug_assert(message, assertion, logic) if((assertion)==logic)\
	{debug_printf("%s failed in %s:%d\n", message, __FILE__, __LINE__);}

#endif // !DEBUG_ASSERT_EN
#define event_printf(message, eventx, logic) if((eventx)==logic)\
    {debug_printf("Event%s: is OK!\n", message);}
#else
#define MCU_PACK __attribute__((packed))
//#define MCU_NAKED __attribute__((naked))
#define MCU_ALIGN(x) __attribute__((aligned (x)))
//#define MCU_ALWAYS_INLINE __attribute__((always_inline))
//#define MCU_NEVER_INLINE __attribute__((noinline))
//#define FNCT_NO_RETURN __attribute__((noreturn))
#define debug_printf(x, ...)
#if (DEBUG_ASSERT_EN==1)
#define debug_assert(message, assertion, logic) if(assertion == logic) while(1)
#endif // !DEBUG_ASSERT_EN
#endif // !defined __MINGW32 elif defined DEBUG_ON_VS  else

#if (DEBUG_ASSERT_EN == 0)
#define debug_assert(message, assertion, logic)
#endif // !DEBUG_ASSERT_EN

/*!!! @TODO:
debug_printf()
#define debug_assert(message, assertion) if(NOT (assertion))\
	{debug_printf("%s failed in %s:%d", message, __FILE__, __LINE__);}

or
#define debug_assert(message, assertion, logic) if((assertion)==logic)\
	{debug_printf("%s failed in %s:%d", message, __FILE__, __LINE__);}

*/

#endif// !TYPE_DEF_H
