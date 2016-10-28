#ifndef _SCHEDULE_CPP
#define _SCHEDULE_CPP
#include "Schedule.h"
Schedule Schedule1;
void Schedule::SCH_Dispatch_Tasks(void) 
{
  tByte Index;

  // Dispatches (runs) the next task (if one is ready)
  for (Index = 0; Index < SCH_MAX_TASKS; Index++)
  {
    if (SCH_tasks_G[Index].RunMe > 0) 
    {
      (*SCH_tasks_G[Index].pTask)();  // Run the task
      SCH_tasks_G[Index].RunMe = 0;   // Reset / reduce RunMe flag
      if (SCH_tasks_G[Index].Period == 0)
      {
        SCH_Delete_Task(Index);
      }
    }
  }
  SCH_Go_To_Sleep();          
}

tByte Schedule::SCH_Add_Task(void (* pFunction)(), const tWord DELAY, const tWord PERIOD)    
{
  tByte Index = 0;
  while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
  {
    Index++;
  } 
  if (Index == SCH_MAX_TASKS)
  {
    return SCH_MAX_TASKS;  
  }

  SCH_tasks_G[Index].pTask  = pFunction;
  SCH_tasks_G[Index].Delay  = DELAY;
  SCH_tasks_G[Index].Period = PERIOD;
  SCH_tasks_G[Index].RunMe  = 0;

  return Index; // return position of task (to allow later deletion)
}

/*------------------------------------------------------------------*-
 
 SCH_Delete_Task()
 
 Removes a task from the scheduler.  Note that this does
 *not* delete the associated function from memory: 
 it simply means that it is no longer called by the scheduler. 
 
 TASK_INDEX - The task index.  Provided by SCH_Add_Task(). 
 
 RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL
 
 -*------------------------------------------------------------------*/
tByte Schedule::SCH_Delete_Task(const tByte TASK_INDEX) 
{
  tByte Return_code;

  if (SCH_tasks_G[TASK_INDEX].pTask == 0)
  {

    // ...also return an error code
    Return_code = RETURN_ERROR;
  }
  else
  {
    Return_code = RETURN_NORMAL;
  }      

  SCH_tasks_G[TASK_INDEX].pTask   = 0x0000;
  SCH_tasks_G[TASK_INDEX].Delay   = 0;
  SCH_tasks_G[TASK_INDEX].Period  = 0;

  SCH_tasks_G[TASK_INDEX].RunMe   = 0;

  return Return_code;       // return status
}
void Schedule::SCH_Go_To_Sleep()
{
  //set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here 
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sleep_mode();    
  sleep_disable();
}
void Schedule::SCH_Update(void) 
{
  tByte Index;

  // NOTE: calculations are in *TICKS* (not milliseconds)
  for (Index = 0; Index < SCH_MAX_TASKS; Index++)
  {
    if (SCH_tasks_G[Index].pTask)
    {
      if (SCH_tasks_G[Index].Delay == 0)
      {
        SCH_tasks_G[Index].RunMe = 1;  // Set the run flag
        if (SCH_tasks_G[Index].Period)
        {
          SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
        }
      }
      else
      {
        SCH_tasks_G[Index].Delay -= 1;
      }
    }         
  }
}   
#endif
