/*
 * APP.h
 *
 *  Created on: Sep 13, 2023
 *      Author: Kareem Abdelrasheed
 */

#ifndef APPLICATION_APP_H_
#define APPLICATION_APP_H_

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "../LIB/bitwise_operation.h"
#include "../LIB/std_types.h"

#include "../HAL/LCD/lcd.h"
#include "../HAL/LDR/ldr_sensor.h"
#include "../HAL/MOTOR/motor.h"
#include "../HAL/SWITCH/switch.h"
#include "../HAL/ULTRASONIC/ultrasonic_sensor.h"
#include "../HAL/TEMP_SENSOR/temperture_sensor.h"


#include "../MCAL/GPIO/gpio.h"
#include "../MCAL/SYSTICK/systick.h"
#include "../MCAL/general_functions.h"

#include "../SCHEDULER/scheduler.h"


void system_init(void);
/*Robot motion functions*/
void robot_move_forward(void);
void robot_move_stop(void);
void robot_move_left(void);
void robot_move_right(void);
void robot_move_rotate(void);
/*Program tasks*/
void switches_update(void);
void lcd_update(void);
void ultrasonic_update(void);
void ldr_update(void);
void temperature_update(void);
void robot_state(void);

#endif /* APPLICATION_APP_H_ */
