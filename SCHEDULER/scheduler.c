/*******************************************************************************
 *                                                                             *
 * [FILE NAME]:   scheduler.c                                                  *
 *                                                                             *
 * [AUTHOR]:      Ahmed Saeed, Amr Gafar, Kareem Abd-elrasheed,                *
 *                Mohamed Abosreea, Ahmed Maher, and Mohamed Ramadan           *
 *                                                                             *
 * [Version]:     1.0.0                                                        *
 *                                                                             *
 * [DATE]:        09/09/2023                                                   *
 *                                                                             *
 * [DESCRIPTION]: source file for scheduler driver                             *
 *                                                                             *
 *******************************************************************************/

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "scheduler.h"
#include "LIB/bitwise_operation.h"
#include "MCAL/systick.h"

/* Define an array of task_t structures to store information about tasks */
task_t tasks[NUMBER_OF_TASKS] = {0};

/* Define an array to store the execution time of each task */
uint16_t tasks_time[NUMBER_OF_TASKS];

/* Define a static systick_t configuration structure named 'systick1' */
static systick_t systick1 = {
    /* Set the SysTick timer clock source to the system clock */
    .systickClk = systickSystemClk,

    /* Enable SysTick interrupts */
    .systickInt = systickIntEnable,

    /* Set the SysTick interrupt handler function to 'tasks_scheduler' */
    .systickHandler = tasks_scheduler
};

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
void init_scheduler(uint32_t tick_time)
{
    /* Initialize the SysTick timer configuration */
    systick_init(&systick1);

    /* Set a periodic interval for the SysTick timer */
    systick_set_periodic_interval(&systick1, tick_time);
}

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
void create_task (void (*Task)(void), uint32_t ms_periodicity , uint8_t first_delay)
{
    /* Define a static variable to keep track of the task's priority */
    static uint8_t task_priority = 0;

    /* Check if the 'Task' pointer is NULL or if 'task_priority' exceeds the maximum number of tasks */
    if (Task == NULL || task_priority >= NUMBER_OF_TASKS)
    {
        /* Return without creating the task if the conditions are not met */
        return;
    }

    else
    {
        /* Store the task function pointer in the 'Task_Handler' field of the tasks array */
        tasks[task_priority].Task_Handler = Task;

        /* Store the task's periodicity in the 'periodicity' field of the tasks array */
        tasks[task_priority].periodicity = ms_periodicity;

        /* Store the initial delay in the 'first_delay' field of the tasks_time array */
        tasks_time[task_priority] = first_delay;

        /* Increment the task priority to prepare for the next task creation */
        task_priority++;
    }
}

/*
 * Description:
 * This function serves as a task scheduler, responsible for managing and executing tasks
 * at their specified periodicities. It iterates through the list of tasks, decrements their
 * countdown timers, and runs a task if its timer reaches zero.
 * Parameters: None.
 * Returns: None.
 */
void tasks_scheduler(void)
{
       /* Counter for the for loop */
       uint8_t counter;

       /* Loop through all tasks */
       for (counter = 0 ; counter < NUMBER_OF_TASKS ; counter++)
       {
           /* Check if a valid task handler is assigned to the task */
           if (tasks[counter].Task_Handler != 0)
           {
               /* Check if the task's countdown timer has reached zero */
               if (tasks_time[counter] == 0)
               {
                   /* Run the task */
                   tasks[counter].Task_Handler();

                   /* Reload the task's periodicity as the countdown timer */
                   tasks_time[counter] = tasks[counter].periodicity ;
               }

               else
               {
                   /* Decrement the countdown timer for the task */
                   tasks_time[counter]-- ;
               }
           }

           else
           {
               /*Do Nothing*/
           }
       }
}


