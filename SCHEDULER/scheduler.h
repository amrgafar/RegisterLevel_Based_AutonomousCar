/*******************************************************************************
 *                                                                             *
 * [FILE NAME]:   scheduler.h                                                  *
 *                                                                             *
 * [AUTHOR]:      Ahmed Saeed, Amr Gafar, Kareem Abd-elrasheed,                *
 *                Mohamed Abosreea, Ahmed Maher, and Mohamed Ramadan           *
 *                                                                             *
 * [Version]:     1.0.0                                                        *
 *                                                                             *
 * [DATE]:        09/09/2023                                                   *
 *                                                                             *
 * [DESCRIPTION]: header file for scheduler driver                             *
 *                                                                             *
 *******************************************************************************/

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "LIB/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define NUMBER_OF_TASKS     6

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef struct
{
    void (*Task_Handler)(void);
    uint16_t periodicity;
    uint32_t First_Delay;
}task_t;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * This function initializes a scheduler using the SysTick timer. It calls
 * 'systickInit' to configure the SysTick timer with the provided 'systick1'
 * configuration structure.
 * Parameters:
 * - tick_time: The time interval (in milliseconds) at which the scheduler
 *              should generate interrupts.
 * Returns: None.
 */
void init_scheduler(uint32_t tick_time);

/*
 * Description:
 * This function creates a task by initializing its properties, such as the task handler function,
 * execution periodicity, and initial delay. The task is added to the tasks array for scheduling.
 * Parameters:
 * - Task: A pointer to the task function to be executed.
 * - ms_periodicity: The execution periodicity of the task in milliseconds.
 * - first_delay: The initial delay before the task's first execution in milliseconds.
 * Returns: None.
 */
void create_task (void (*Task)(void), uint32_t ms_periodicity , uint8_t first_delay);

/*
 * Description:
 * This function serves as a task scheduler, responsible for managing and executing tasks
 * at their specified periodicities. It iterates through the list of tasks, decrements their
 * countdown timers, and runs a task if its timer reaches zero.
 * Parameters: None.
 * Returns: None.
 */
void tasks_scheduler(void) ;

/* void sys_handler(void); */

#endif /* SCHEDULER_H_ */
