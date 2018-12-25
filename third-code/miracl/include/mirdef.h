#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
/*
*   MIRACL compiler/hardware definitions - mirdef.h
*/

#define MR_LITTLE_ENDIAN
#define MIRACL 64
#define mr_utype __int64
#define mr_unsign64 unsigned __int64
#define MR_IBITS 32
#define MR_LBITS 32
#define mr_unsign32 unsigned int
#define MR_FLASH 52
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8
#define MR_WINDOWS_MT 1


#else//linux or mac
 /*
 *   MIRACL compiler/hardware definitions - mirdef.h
 */

#define MR_LITTLE_ENDIAN
#define MIRACL 64
#define mr_utype long
#define mr_unsign64 unsigned long
#define MR_IBITS 32
#define MR_LBITS 64
#define mr_unsign32 unsigned int
#define MR_FLASH 52
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8
#define MR_UNIX_MT 1

#endif

