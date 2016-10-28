#ifndef _SCHEDULE_H
#define _SCHEDULE_H
#include "main.h"
#include <avr/sleep.h>
struct sTask
{
	void (* pTask)(void);  
	tWord Delay;       
	tWord Period;       
	tByte RunMe;       
}; 
#define SCH_MAX_TASKS   (5)   
class Schedule
{
public:
	sTask SCH_tasks_G[SCH_MAX_TASKS];
	static tWord Error_tick_count_G;
	static tByte Last_error_code_G;

	void SCH_Update(void);
	void  SCH_Dispatch_Tasks(void);
	tByte SCH_Add_Task(void (*) (void), const tWord, const tWord);  
	tByte SCH_Delete_Task(const tByte);
	static void SCH_Go_To_Sleep(void);
};
extern Schedule Schedule1;
#endif
