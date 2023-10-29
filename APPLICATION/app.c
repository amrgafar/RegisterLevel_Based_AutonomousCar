/*******************************************************************************
 *                                                                             *
 * [FILE NAME]:   app.c                                                       *
 *                                                                             *
 * [AUTHOR]:      Ahmed Saeed, Amr Gafar, Kareem Abd-elrasheed,                *
 *                Mohamed Abosreea, Ahmed Maher, and Mohamed Ramadan           *
 *                                                                             *
 * [Version]:     1.0.0                                                        *
 *                                                                             *
 * [DATE]:        09/09/2023                                                   *
 *                                                                             *
 * [DESCRIPTION]: This file contain the hardware connection of project         *
 *                                                                             *
 *******************************************************************************/
#include "app.h"

/*Global variables*/
uint16_t LDR1_Read = 0;
uint16_t LDR2_Read = 0;
uint16_t LDR_Difference = 0;
uint16_t obstacle_distance = 0 ;
uint8_t start_flag = 0;
uint8_t stop_flag = 0;
uint16_t temperature_value = 0 ;


button_t start_switch = {
                         .btnPort =PORTF_ID,
                         .btnPin = PIN0_ID,
                         .btn_conn = PULL_UP,
                         .btn_state = RELEASED
};

button_t stop_switch = {
                        .btnPort =PORTF_ID,
                        .btnPin = PIN4_ID,
                        .btn_conn = PULL_UP,
                        .btn_state = RELEASED
};

carSide_t LEFT_SIDE  = {
                        .carSidePort = PORTB_ID,
                        .carSideDir1Pin = PIN7_ID,
                        .carSideDir2Pin = PIN1_ID,
                        .carSideEnPin = PIN6_ID,
                        .carSideSpeed = 30

};

carSide_t RIGHT_SIDE = {
                        .carSidePort = PORTB_ID,
                        .carSideDir1Pin = PIN4_ID,
                        .carSideDir2Pin = PIN5_ID,
                        .carSideEnPin = PIN0_ID,
                        .carSideSpeed = 30
};

LDR_Sensor_t LDR1 = {
                     .ldrPort = PORTE_ID,
                     .ldrPin = PIN1_ID,
                     .adcChannel = CH_1,
                     .sequencer = SS_0,
                     .sample = S_0,
                     .adc_module = ADC_0,
                     .temp_sensor_mode =NO

};

LDR_Sensor_t LDR2 = {
                     .ldrPort = PORTE_ID,
                     .ldrPin = PIN2_ID,
                     .adcChannel = CH_2,
                     .sequencer = SS_1,
                     .sample = S_0,
                     .adc_module = ADC_1,
                     .temp_sensor_mode =NO
};

Temp_Sensor_t TEMP_SENSOR = {
                             .ldrPort = PORTE_ID,
                             .ldrPin = PIN3_ID,
                             .adcChannel = CH_0,
                             .sequencer = SS_3,
                             .sample = S_0,
                             .adc_module = ADC_1,
                             .temp_sensor_mode =YES

};

ultraSonic_t Ultrasonic_Sensor = {
                                  .ultrasonicPort = PORTC_ID,
                                  .ultrasonicTriggerPin = PIN5_ID,
                                  .ultrasonicEchoPin = PIN6_ID
};


void system_init(void)
{
    /*car side configuration initialization*/
    car_side_init(&LEFT_SIDE , &RIGHT_SIDE);
    /*LDR init*/
    ldr_sensor_init(&LDR1);
    ldr_sensor_init(&LDR2);
    /*TEMP_Sensor init*/
    temperature_sensor_init(&TEMP_SENSOR);
    /*Ultrasonic init*/
    ultarsonic_sensor_init(&Ultrasonic_Sensor);
    /*initialization of lcd */
    lcd_init();
    /*start switch init*/
    button_init(&start_switch);
    /*stop switch init*/
    button_init(&stop_switch);
}
void robot_move_forward(void)
{
    car_side_direction_set(&LEFT_SIDE , forward);
    car_side_direction_set(&RIGHT_SIDE , forward);
}
void robot_move_stop(void)
{
    car_side_direction_set(&LEFT_SIDE , stop);
    car_side_direction_set(&RIGHT_SIDE , stop);
}
void robot_move_left(void)
{
    car_side_direction_set(&LEFT_SIDE , forward);
    car_side_direction_set(&RIGHT_SIDE , stop);

}
void robot_move_right(void)
{
    car_side_direction_set(&LEFT_SIDE , stop);
    car_side_direction_set(&RIGHT_SIDE , forward);
}
void robot_move_rotate(void)
{
    car_side_direction_set(&LEFT_SIDE , backward);
    car_side_direction_set(&RIGHT_SIDE , forward);
    _delay_ms(300);
    car_side_direction_set(&LEFT_SIDE , stop);
    car_side_direction_set(&RIGHT_SIDE , stop);
}

void switches_update(void)
{
    static buttonState_t current_start_switch_state , last_start_switch_state = RELEASED;
    static buttonState_t current_stop_switch_state , last_stop_switch_state = RELEASED;

    button_get_state(&start_switch, &current_start_switch_state);
    button_get_state(&stop_switch, &current_stop_switch_state);

    /****************** Start operation************************/
    /*when both current and last are equal do nothing*/
    if(current_start_switch_state == PRESSED && last_start_switch_state == RELEASED)
    {
        start_flag = 1 ;
        stop_flag = 0 ;
    }
    else /*Button_currentState == RELEASED && Button_lastState == PRESSED*/
    {
        /*then we are in the pre-release state */
        /*make the last stated = pressed*/
        last_start_switch_state = current_start_switch_state;
    }
    /****************** Stop operation************************/
    if(current_stop_switch_state == PRESSED && last_stop_switch_state == RELEASED)
    {
        start_flag = 0U ;
        stop_flag = 1U ;
    }
    else /*Button_currentState == RELEASED && Button_lastState == PRESSED*/
    {
        /*then we are in the pre-release state */
        /*make the last stated = pressed*/
        last_stop_switch_state = current_stop_switch_state;
    }
}
void lcd_update(void)
{
    if(start_flag == 1U)
    {
        /*Set cursor position to ROW_0 and COL_0*/
        lcd_move_cursor(0,0);
        /*Displaying Temperature sensor value*/
        lcd_display_string("Temp = ");
        lcd_intger_to_string(temperature_value);
        lcd_display_string(" C");
        /*Displaying Time */

        /*Displaying LDR difference value*/
        lcd_move_cursor(1,10);
        lcd_intger_to_string(LDR_Difference);
    }
    else{
        /*Do Nothing*/
    }
}
void ultrasonic_update(void)
{
    if(start_flag == 1U)
    {
        /*get distance*/
        obstacle_distance = ultarsonic_sensor_read_value(&Ultrasonic_Sensor);
        /*check the car is blocked*/
        if((10U < obstacle_distance) || (0U == obstacle_distance ))
        {
            robot_move_forward();
        }
        else
        {
            robot_move_rotate();
        }
    }else{
        /*Do Nothing*/
    }
}
void ldr_update(void)
{
    if(start_flag == 1U)
    {
        LDR1_Read = ldr_sensor_reading(&LDR1);
        LDR2_Read = ldr_sensor_reading(&LDR2);

        LDR_Difference = LDR2_Read - LDR1_Read;
        /*check LDRs value*/
        if( (400U > LDR_Difference))
        {
            robot_move_forward();
        }
        else
        {

            if(0<LDR_Difference)
            {
                robot_move_right();
            }
            else
            {
                robot_move_left();
            }
            LDR_Difference = 0U;
        }
    }else{
        /*Do Nothing*/
    }
}

void robot_state(void)
{
    if(stop_flag == 1U)
    {
        robot_move_stop();
    }else{
        /*Do Nothing*/
    }
}
void temperature_update(void)
{
    if(start_flag == 1U)
    {
        temperature_value = temperature_sensor_reading(&TEMP_SENSOR);
        temperature_value = (1475 - ((750*33*temperature_value)/4096));
    }else{
        /*Do Nothing*/
    }
}
