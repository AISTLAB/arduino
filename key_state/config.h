#ifndef CONFIG_H_
#define CONFIG_H_

#include<reg52.h>

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

/*type define*/
typedef unsigned char boolean;
typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned int uint16;
typedef signed int   sint16;
typedef unsigned long uint32;
typedef signed long  sint32;
typedef float        fp32;

/*系统时钟*/
#define Fosc   11059200 		/*HZ					*/
#define PER_INS_CYC    12  	/*12时钟周期为一个机器周期*/

#endif


