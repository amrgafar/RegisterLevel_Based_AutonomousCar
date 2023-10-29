/*******************************************************************************
 *                                                                             *
 * [FILE NAME]:   systick.h                                                    *
 *                                                                             *
 * [AUTHOR]:      Ahmed Saeed, Amr Gafar, Kareem Abd-elrasheed,                *
 *                Mohamed Abosreea, Ahmed Maher, and Mohamed Ramadan           *
 *                                                                             *
 * [Version]:     1.0.0                                                        *
 *                                                                             *
 * [DATE]:        09/09/2023                                                   *
 *                                                                             *
 * [DESCRIPTION]: header file for Systick driver                               *
 *                                                                             *
 *******************************************************************************/

#ifndef SYSTICK_H_
#define SYSTICK_H_

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "LIB/std_types.h"
#include "TM4C123GH6PM.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define F_CPU   16000000U

/*******************************************************************************
 *                            Macros like Function                             *
 *******************************************************************************/
/* Enable the SysTick timer. */
#define SYSTICK_ENABLE()             ((NVIC_ST_CTRL_R) |= (1U<<0))

/* Disable the SysTick timer. */
#define SYSTICK_DISABLE()            ((NVIC_ST_CTRL_R) &= ~(1U<<0))

/* Enable interrupts for the SysTick timer. */
#define SYSTICK_INTERRUPT_ENABLE()   ((NVIC_ST_CTRL_R) |= (1U<<1))

/* Disable interrupts for the SysTick timer. */
#define SYSTICK_INTERRUPT_DISABLE()  ((NVIC_ST_CTRL_R) &= ~(1U<<1))

/* Select the internal clock source for the SysTick timer. */
#define SYSTICK_INTERNAL_CLK_SRC()   ((NVIC_ST_CTRL_R) &= ~(1U<<2))

/* Select the system clock source for the SysTick timer. */
#define SYSTICK_SYSTEM_CLK_SRC()     ((NVIC_ST_CTRL_R) |= (1U<<2))

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/*
 * Description :
 * Enum for SysTick interrupt enable/disable options.
 */
typedef enum {
    systickIntDisable,
    systickIntEnable
} systickInterrupt_t;

/*
 * Description :
 * Enum for SysTick clock source options.
 */
typedef enum {
    systicInternalCLk_4,
    systickSystemClk
} systickClkSrc_t;

/*
 * Description :
 * Enum for error checking status.
 */
typedef enum {
    N_OK,
    OK
} Err_check_t;

/*
 * Description :
 * Struct responsible for configuring SysTick settings.
 */
typedef struct {
    systickInterrupt_t systickInt;
    systickClkSrc_t systickClk;
    void (*systickHandler)(void);
} systick_t;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
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
Err_check_t systick_init(systick_t *systickConfig);

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
Err_check_t systick_deinit(systick_t *systickConfig);

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
Err_check_t systick_set_busy_wait(systick_t *systickConfig , uint32_t secMs);

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
Err_check_t systick_set_single_interval(systick_t *systickConfig , uint32_t secMs);

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
Err_check_t systick_set_periodic_interval(systick_t *systickConfig , uint32_t secMs);


#endif /* SYSTICK_H_ */
