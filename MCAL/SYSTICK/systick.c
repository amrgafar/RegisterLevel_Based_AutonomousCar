/*******************************************************************************
 *                                                                             *
 * [FILE NAME]:   systick.c                                                    *
 *                                                                             *
 * [AUTHOR]:      Ahmed Saeed, Amr Gafar, Kareem Abd-elrasheed,                *
 *                Mohamed Abosreea, Ahmed Maher, and Mohamed Ramadan           *
 *                                                                             *
 * [Version]:     1.0.0                                                        *
 *                                                                             *
 * [DATE]:        09/09/2023                                                   *
 *                                                                             *
 * [DESCRIPTION]: source file for Systick driver                               *
 *                                                                             *
 *******************************************************************************/

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "systick.h"
#include "LIB/bitwise_operation.h"

/* Pointer to a function with no arguments and no return value. Initialized as NULL */
static void (*funcPtr)(void) = NULL;

/* Flag indicating the SysTick mode */
static uint8_t systickModeFlag;

/*
 * Description :
 * This function initializes the SysTick timer module, configuring its
 * interrupt, clock source, and error checking. If 'systickConfig' is NULL,
 * it updates 'e_check' to 'N_OK', indicating an error. Otherwise, it
 * disables the SysTick Timer, clears the Current Value register, manages
 * the interrupt and clock source settings based on 'systickConfig', and
 * returns the error check result.
 * Parameters:
 * - systickConfig: Pointer to a systick_t structure containing
 *                  configuration settings.
 * Returns:
 * - Err_check_t: Enum indicating whether the initialization was
 *                successful (OK) or encountered an error (N_OK).
 */
Err_check_t systick_init(systick_t *systickConfig)
{
    /* Initialize an error-checking variable 'e_check' to 'OK' */
    Err_check_t e_check = OK;

    /* Check if 'systickConfig' is NULL and update 'e_check' to 'N_OK' if true */
    if(NULL == systickConfig)
    {
        e_check = N_OK;
    }

    else
    {
        /* Disable the SysTick Timer */
        SYSTICK_DISABLE();
        /* Clear Current Value register by writing a value */
        NVIC_ST_CURRENT_R = 0 ;
        /* Manage Interrupt [Enable or Disable] */
        MODIFY_BIT(NVIC_ST_CTRL_R , 1 ,systickConfig->systickInt);
        /* Manage Clock Source [Internal Clock div by 4 - System CLock] */
        MODIFY_BIT(NVIC_ST_CTRL_R , 2 ,systickConfig->systickClk);
    }

    /* Return the error check result */
    return e_check;
}

/*
 * Description :
 * This function deinitializes the SysTick timer module, disabling the timer,
 * its interrupt, and resetting its current and reload values. If 'systickConfig'
 * is NULL, it updates 'e_check' to 'N_OK', indicating an error. Otherwise, it
 * performs the deinitialization steps for the SysTick timer based on 'systickConfig'
 * and returns the error check result.
 * Parameters:
 * - systickConfig: Pointer to a systick_t structure containing
 *                  configuration settings.
 * Returns:
 * - Err_check_t: Enum indicating whether the deinitialization was
 *                successful (OK) or encountered an error (N_OK).
 */
Err_check_t systick_deinit(systick_t *systickConfig)
{
    /* Initialize an error-checking variable 'e_check' to 'OK' */
    Err_check_t e_check = OK;

    /* Check if 'systickConfig' is NULL and update 'e_check' to 'N_OK' if true */
    if(NULL == systickConfig)
    {
        e_check = N_OK;
    }

    else
    {
        /* Disable the SysTick timer */
        SYSTICK_DISABLE();

        /* Disable SysTick interrupt */
        SYSTICK_INTERRUPT_DISABLE();

        /* Reset the SysTick current value */
        NVIC_ST_CURRENT_R = 0 ;

        /* Reset the SysTick reload value */
        NVIC_ST_RELOAD_R = 0;
    }

    /* Return the error check result */
    return e_check;
}

/*
 * Description :
 * This function provides a busy-wait delay functionality using the SysTick timer.
 * It initializes the SysTick timer based on the provided 'systickConfig' and
 * generates a delay of 'secMs' milliseconds. If 'systickConfig' is NULL, it
 * updates 'e_check' to 'N_OK', indicating an error. Otherwise, it configures
 * and utilizes the SysTick timer to create the desired delay.
 * Parameters:
 * - systickConfig: Pointer to a systick_t structure containing
 *                  configuration settings for the SysTick timer.
 * - secMs: The delay time in milliseconds.
 * Returns:
 * - Err_check_t: Enumeration indicating whether the operation was
 *                successful (OK) or encountered an error (N_OK).
 */
Err_check_t systick_set_busy_wait(systick_t *systickConfig , uint32_t secMs)
{
    /* Initialize an error-checking variable 'e_check' to 'OK' */
    Err_check_t e_check = OK;

    /* Check if 'systickConfig' is NULL and update 'e_check' to 'N_OK' if true */
    if(NULL == systickConfig)
    {
        e_check = N_OK;
    }

    else
    {

        /* Disable the SysTick timer */
        SYSTICK_DISABLE();

        /* Disable SysTick interrupt */
        SYSTICK_INTERRUPT_DISABLE();

        /* Configure SysTick to use the system clock as the source */
        SYSTICK_SYSTEM_CLK_SRC();

        /* Calculate the reload value based on the desired delay in milliseconds */
        uint32_t value = (float32_t)(secMs) * F_CPU / 1000.0;

        /* Set the preload value for SysTick */
        NVIC_ST_RELOAD_R = value - 1;

        /* Clear the current value register by writing a value */
        NVIC_ST_CURRENT_R = 0;

        /* Enable the SysTick timer */
        SYSTICK_ENABLE();

        /* Wait for the COUNT flag in the SysTick control and status register to become 1 */
        while (GET_BIT(NVIC_ST_CTRL_R, 16) == 0);

        /* Clear the current value register again */
        NVIC_ST_CURRENT_R = 0;
    }

    /* Return the error check result */
    return e_check;
}

/*
 * Description :
 * This function configures the SysTick timer for single interval mode.
 * It sets the provided 'systickConfig' to operate in single interval mode,
 * generating a delay of 'secMs' milliseconds. If 'systickConfig' is NULL, it
 * updates 'e_check' to 'N_OK', indicating an error. Otherwise, it configures
 * and utilizes the SysTick timer with the specified settings.
 * Parameters:
 * - systickConfig: Pointer to a systick_t structure containing
 *                  configuration settings for the SysTick timer.
 * - secMs: The delay time in milliseconds.
 * Returns:
 * - Err_check_t: Enum indicating whether the operation was
 *                successful (OK) or encountered an error (N_OK).
 */
Err_check_t systick_set_single_interval(systick_t *systickConfig , uint32_t secMs)
{
    /* Initialize an error-checking variable 'e_check' to 'OK' */
    Err_check_t e_check = OK;

    /* Check if 'systickConfig' is NULL and update 'e_check' to 'N_OK' if true */
    if(NULL == systickConfig)
    {
        e_check = N_OK;
    }

    else
    {
        /* Set the systickModeFlag to 1 */
        systickModeFlag = 1;

        /* Disable the SysTick timer */
        SYSTICK_DISABLE();

        /* Enable SysTick interrupt */
        SYSTICK_INTERRUPT_ENABLE();

        /* Set Clock Source: Internal Clock div by 4 or System Clock */
        MODIFY_BIT(NVIC_ST_CTRL_R, 2, systickConfig->systickClk);

        /* Calculate the reload value based on the desired delay in milliseconds */
        uint32_t value = (float32_t)(secMs) * F_CPU / 1000.0;

        /* Set the preload value for SysTick */
        NVIC_ST_RELOAD_R = value - 1;

        /* Clear the current value register by writing a value */
        NVIC_ST_CURRENT_R = 0;

        /* Check if a custom SysTick handler function is provided */
        if (systickConfig->systickHandler != NULL)
        {
            /* Set the function pointer to the custom handler */
            funcPtr = systickConfig->systickHandler;
        }

        else
        {
            /* If no handler is provided, set an error flag and disable SysTick interrupt */
            e_check = N_OK;
            SYSTICK_INTERRUPT_DISABLE();
        }

        /* Enable the SysTick timer */
        SYSTICK_ENABLE();
    }

    /* Return the error check result */
    return e_check;
}

/*
 * Description :
 * This function configures the SysTick timer for periodic interval mode.
 * It sets the provided 'systickConfig' to operate in periodic interval mode,
 * generating a periodic delay of 'secMs' milliseconds. If 'systickConfig' is NULL,
 * it updates 'e_check' to 'N_OK', indicating an error. Otherwise, it configures
 * and utilizes the SysTick timer with the specified settings.
 * Parameters:
 * - systickConfig: Pointer to a systick_t structure containing
 *                  configuration settings for the SysTick timer.
 * - secMs: The delay time in milliseconds for each period.
 * Returns:
 * - Err_check_t: Enum indicating whether the operation was
 *                successful (OK) or encountered an error (N_OK).
 */
Err_check_t systick_set_periodic_interval(systick_t *systickConfig , uint32_t secMs)
{
    /* Initialize an error-checking variable 'e_check' to 'OK' */
    Err_check_t e_check = OK;

    /* Check if 'systickConfig' is NULL and update 'e_check' to 'N_OK' if true */
    if(NULL == systickConfig)
    {
        e_check = N_OK;
    }

    else
    {
        /* Disable the SysTick timer */
        SYSTICK_DISABLE();

        /* Enable SysTick interrupt */
        SYSTICK_INTERRUPT_ENABLE();

        /* Set Clock Source: Internal Clock div by 4 or System Clock */
        MODIFY_BIT(NVIC_ST_CTRL_R , 2 ,systickConfig->systickClk);

        /* Calculate the reload value based on the desired delay in milliseconds */
        uint32_t value = (float32_t)(secMs) * F_CPU/1000.0;

        /* Set the preload value for SysTick */
        NVIC_ST_RELOAD_R = value - 1;

        /* Clear the current value register by writing a value */
        NVIC_ST_CURRENT_R = 0;

        /* Check if a custom SysTick handler function is provided */
        if (systickConfig->systickHandler != NULL)
        {
            /* Set the function pointer to the custom handler */
            funcPtr = systickConfig->systickHandler;
        }

        else
        {
            /* If no handler is provided, set an error flag and disable SysTick interrupt */
            e_check = N_OK;
            SYSTICK_INTERRUPT_DISABLE();
        }

        /* Enable the SysTick timer */
        SYSTICK_ENABLE();
    }

    /* Return the error check result */
    return e_check;
}

/*
 * Description :
 * This SysTick interrupt handler function is responsible for handling
 * SysTick timer interrupts. It checks if the 'funcPtr' function pointer
 * is not NULL and the 'systickModeFlag' is set to 1. If both conditions
 * are met, it disables the SysTick timer, clears the current and reload
 * value registers, and resets the 'systickModeFlag'. Then, it calls the
 * function pointed to by 'funcPtr'. If 'funcPtr' is NULL or 'systickModeFlag'
 * is not set, it performs no action.
 */
void Systick_Handler(void)
{
    if (funcPtr != NULL)
    {
        /* Check if the systickModeFlag is set to 1 */
        if (systickModeFlag == 1)
        {
            /* Disable the SysTick timer */
            SYSTICK_DISABLE();

            /* Disable SysTick interrupt */
            SYSTICK_INTERRUPT_DISABLE();

            /* Reset the current value register */
            NVIC_ST_CURRENT_R = 0;

            /* Reset the reload value register */
            NVIC_ST_RELOAD_R = 0;

            /* Reset the systickModeFlag */
            systickModeFlag = 0;
        }
        else
        {
            /* Do Nothing */
        }

        /* Call the function pointed to by funcPtr */
        funcPtr();
    }
    else
    {
        /* Do Nothing */
    }

}

