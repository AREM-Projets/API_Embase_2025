/*
 * Motors.h
 *
 *  Created on: Oct 12, 2024
 *      Author: Lenovo
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>

// Incremetnation period for all motor timers
#define MOT_TIM_TICKTIME_US 1

#define MOT0_TIM_HANDLE_PTR (&htim2)
#define MOT1_TIM_HANDLE_PTR (&htim6)
#define MOT2_TIM_HANDLE_PTR (&htim7)

typedef enum
{
	DIR_FORWARD = 0,
	DIR_BACKWARD = 1
} MotorDirection_t;

typedef struct
{
	uint32_t step_period_us;
	MotorDirection_t direction;
	bool enabled;
} MotorState_t;

/* The states of all 3 motors are defined in Motors.c as the following variables :
 * - Motor0_state (at the front of the robot)
 * - Motor1_state (back left)
 * - Motor2_state (back right)
 * They cannot be accessed from outside the Motors.c file (static variables) but can be controlled through the below functions.
 *
 * The pins associated with the motors should be named in the .ioc with the following names :
 * - MOTX_EN, MOTX_STP, MOTX_DIR (replace X with 0, 1 and 2)
 */

void Motors_Init();

void Motors_SetSpeedSingle(uint8_t motor_index, MotorDirection_t direction, uint32_t period_us);
void Motors_StopSingle(uint8_t motor_index);

void Motors_SetSpeedAll(MotorDirection_t *directions, uint32_t *periods_us);
void Motors_StopAll();

void Motors_Update();

// Callback should be added to the HAL_TIM_PeriodElapsedCallback
void Motors_TimerITCallback(TIM_HandleTypeDef* htim);

#endif /* INC_MOTORS_H_ */
