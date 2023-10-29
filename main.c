/*******************************************************************************
 *                                                                             *
 * [FILE NAME]:   main.c                                                       *
 *                                                                             *
 * [AUTHOR]:      Ahmed Saeed, Amr Gafar, Kareem Abd-elrasheed,                *
 *                Mohamed Abosreea, Ahmed Maher, and Mohamed Ramadan           *
 *                                                                             *
 * [Version]:     1.0.0                                                        *
 *                                                                             *
 * [DATE]:        09/09/2023                                                   *
 *                                                                             *
 * [DESCRIPTION]:                                                              *
 *                                                                             *
 *******************************************************************************/
#include "APPLICATION/app.h"
///*
///*Global variables*/
//uint16_t LDR1_Read = 0;
//uint16_t LDR2_Read = 0;
//uint16_t LDR_Difference = 0;
//uint16_t obstacle_distance = 0 ;
//uint8_t start_flag = 0;
//uint8_t stop_flag = 0;
//uint16_t temperature_value = 0 ;
//
//void system_init(void);
///*Robot motion functions*/
//void robot_move_forward(void);
//void robot_move_stop(void);
//void robot_move_left(void);
//void robot_move_right(void);
//void robot_move_rotate(void);
///*Program tasks*/
//void switches_update(void);
//void lcd_update(void);
//void ultrasonic_update(void);
//void ldr_update(void);
//void temperature_update(void);
//void robot_state(void);
//*/
void main(void)
{
    /*initialization the system*/
    system_init();
    /*initialization of the schedular with tick time 1 ms*/
    init_scheduler(1);

    create_task(switches_update, 120, 0);
    create_task(lcd_update, 40, 0);
    create_task(ldr_update, 20, 0);
    create_task(temperature_update, 100, 0);
    create_task(robot_state, 100, 0);
    create_task(ultrasonic_update, 40, 0);

    tasks_scheduler();
    /**/
    while(1)
    {

    }

}
//
//void system_init(void)
//{
//    /*car side configuration initialization*/
//    car_side_init(&LEFT_SIDE , &RIGHT_SIDE);
//    /*LDR init*/
//    ldr_sensor_init(&LDR1);
//    ldr_sensor_init(&LDR2);
//    /*TEMP_Sensor init*/
//    temperature_sensor_init(&TEMP_SENSOR);
//    /*Ultrasonic init*/
//    ultarsonic_sensor_init(&Ultrasonic_Sensor);
//    /*initialization of lcd */
//    lcd_init();
//    /*start switch init*/
//    button_init(&start_switch);
//    /*stop switch init*/
//    button_init(&stop_switch);
//}
//void robot_move_forward(void)
//{
//    car_side_direction_set(&LEFT_SIDE , forward);
//    car_side_direction_set(&RIGHT_SIDE , forward);
//}
//void robot_move_stop(void)
//{
//    car_side_direction_set(&LEFT_SIDE , stop);
//    car_side_direction_set(&RIGHT_SIDE , stop);
//}
//void robot_move_left(void)
//{
//    car_side_direction_set(&LEFT_SIDE , forward);
//    car_side_direction_set(&RIGHT_SIDE , stop);
//
//}
//void robot_move_right(void)
//{
//    car_side_direction_set(&LEFT_SIDE , stop);
//    car_side_direction_set(&RIGHT_SIDE , forward);
//}
//void robot_move_rotate(void)
//{
//    car_side_direction_set(&LEFT_SIDE , backward);
//    car_side_direction_set(&RIGHT_SIDE , forward);
//    _delay_ms(300);
//    car_side_direction_set(&LEFT_SIDE , stop);
//    car_side_direction_set(&RIGHT_SIDE , stop);
//}
//
//void switches_update(void)
//{
//    static buttonState_t current_start_switch_state , last_start_switch_state = RELEASED;
//    static buttonState_t current_stop_switch_state , last_stop_switch_state = RELEASED;
//
//    button_get_state(&start_switch, &current_start_switch_state);
//    button_get_state(&stop_switch, &current_stop_switch_state);
//
//    /****************** Start operation************************/
//    /*when both current and last are equal do nothing*/
//    if(current_start_switch_state == PRESSED && last_start_switch_state == RELEASED)
//    {
//        start_flag = 1 ;
//        stop_flag = 0 ;
//    }
//    else /*Button_currentState == RELEASED && Button_lastState == PRESSED*/
//    {
//        /*then we are in the pre-release state */
//        /*make the last stated = pressed*/
//        last_start_switch_state = current_start_switch_state;
//    }
//    /****************** Stop operation************************/
//    if(current_stop_switch_state == PRESSED && last_stop_switch_state == RELEASED)
//    {
//        start_flag = 0U ;
//        stop_flag = 1U ;
//    }
//    else /*Button_currentState == RELEASED && Button_lastState == PRESSED*/
//    {
//        /*then we are in the pre-release state */
//        /*make the last stated = pressed*/
//        last_stop_switch_state = current_stop_switch_state;
//    }
//}
//void lcd_update(void)
//{
//    if(start_flag == 1U)
//    {
//        /*Set cursor position to ROW_0 and COL_0*/
//        lcd_move_cursor(0,0);
//        /*Displaying Temperature sensor value*/
//        lcd_display_string("Temp = ");
//        lcd_intger_to_string(temperature_value);
//        lcd_display_string(" C");
//        /*Displaying Time */
//
//        /*Displaying LDR difference value*/
//        lcd_move_cursor(1,10);
//        lcd_intger_to_string(LDR_Difference);
//    }
//    else{
//        /*Do Nothing*/
//    }
//}
//void ultrasonic_update(void)
//{
//    if(start_flag == 1U)
//    {
//        /*get distance*/
//        obstacle_distance = ultarsonic_sensor_read_value(&Ultrasonic_Sensor);
//        /*check the car is blocked*/
//        if((10U < obstacle_distance) || (0U == obstacle_distance ))
//        {
//            robot_move_forward();
//        }
//        else
//        {
//            robot_move_rotate();
//        }
//    }else{
//        /*Do Nothing*/
//    }
//}
//void ldr_update(void)
//{
//    if(start_flag == 1U)
//    {
//        LDR1_Read = ldr_sensor_reading(&LDR1);
//        LDR2_Read = ldr_sensor_reading(&LDR2);
//
//        LDR_Difference = LDR2_Read - LDR1_Read;
//        /*check LDRs value*/
//        if( (400U > LDR_Difference))
//        {
//            robot_move_forward();
//        }
//        else
//        {
//
//            if(0<LDR_Difference)
//            {
//                robot_move_right();
//            }
//            else
//            {
//                robot_move_left();
//            }
//            LDR_Difference = 0U;
//        }
//    }else{
//        /*Do Nothing*/
//    }
//}
//
//void robot_state(void)
//{
//    if(stop_flag == 1U)
//    {
//        robot_move_stop();
//    }else{
//        /*Do Nothing*/
//    }
//}
//void temperature_update(void)
//{
//    if(start_flag = =1U)
//    {
//        temperature_value = temperature_sensor_reading(&TEMP_SENSOR);
//    }else{
//        /*Do Nothing*/
//    }
//}
