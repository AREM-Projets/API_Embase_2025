#include "Motors.h"

/////// Local variables //////

// #define MOTORS_MIN_PERIOD_US 100

/// Max period because we are using 16 bit timers
#define MOT_TIM_MAX_PERIOD_US (65535 / (2 * MOT_TIM_TICKTIME_US))

// Tracks the state of the motors
static MotorState_t motor_states[3];

// Tracks which motor has an enabled IT
static volatile bool motor_it_enabled[3] = {0};

// double of the remaining steps to do for each motor (updated in IT)
static volatile int64_t motor_double_remaining_steps[3] = {0};


/////// Defines and macros ///////

/**
 * Get the handle pointer (&htimx) for a motor based on its index.
 * An invalid index will return NULL
 */
#define MOT_TIM_HANDLE_PTR(motor_index) (((motor_index) == 2) ? MOT2_TIM_HANDLE_PTR : \
										(((motor_index) == 1) ? MOT1_TIM_HANDLE_PTR : \
										(((motor_index) == 0) ? MOT0_TIM_HANDLE_PTR : NULL)))

/**
 * Get the GPIO port (MOTx_DIR_GPIO_Port) for a motor based on its index.
 * An invalid index will return 0
 */
#define MOT_STP_GPIO_PORT(motor_index)  (((motor_index) == 2) ? MOT2_STP_GPIO_Port : \
										(((motor_index) == 1) ? MOT1_STP_GPIO_Port : \
										(((motor_index) == 0) ? MOT0_STP_GPIO_Port : 0)))

/**
 * Get the GPIO pin (MOTx_DIR_Pin) for a motor based on its index.
 * An invalid index will return 0
 */
#define MOT_STP_GPIO_PIN(motor_index)  	(((motor_index) == 2) ? MOT2_STP_Pin : \
										(((motor_index) == 1) ? MOT1_STP_Pin : \
										(((motor_index) == 0) ? MOT0_STP_Pin : 0)))

/**
 * Get the GPIO port (MOTx_DIR_GPIO_Port) for a motor based on its index.
 * An invalid index will return 0
 */
#define MOT_DIR_GPIO_PORT(motor_index)  (((motor_index) == 2) ? MOT2_DIR_GPIO_Port : \
										(((motor_index) == 1) ? MOT1_DIR_GPIO_Port : \
										(((motor_index) == 0) ? MOT0_DIR_GPIO_Port : 0)))

/**
 * Get the GPIO pin (MOTx_DIR_Pin) for a motor based on its index.
 * An invalid index will return 0
 */
#define MOT_DIR_GPIO_PIN(motor_index)  	(((motor_index) == 2) ? MOT2_DIR_Pin : \
										(((motor_index) == 1) ? MOT1_DIR_Pin : \
										(((motor_index) == 0) ? MOT0_DIR_Pin : 0)))

/**
 * Get the GPIO port (MOTx_EN_GPIO_Port) for a motor based on its index.
 * An invalid index will return 0
 */
#define MOT_EN_GPIO_PORT(motor_index)  (((motor_index) == 2) ? MOT2_EN_GPIO_Port : \
										(((motor_index) == 1) ? MOT1_EN_GPIO_Port : \
										(((motor_index) == 0) ? MOT0_EN_GPIO_Port : 0)))

/**
 * Get the GPIO pin (MOTx_EN_Pin) for a motor based on its index.
 * An invalid index will return 0
 */
#define MOT_EN_GPIO_PIN(motor_index)  	(((motor_index) == 2) ? MOT2_EN_Pin : \
										(((motor_index) == 1) ? MOT1_EN_Pin : \
										(((motor_index) == 0) ? MOT0_EN_Pin : 0)))


//////// Local functions declaration //////

void Motors_Update();

/////// Functions definitions //////

/**
 * @brief get the number of remaining steps for a motor (negative value = infinity)
 * 
 * @param motor_index index of the motor
 * @return int64_t 
 */
int64_t Motors_getRemainingSteps(uint8_t motor_index)
{
	switch (motor_double_remaining_steps[motor_index]) {

		case 0:
			return 0;
			break;

		case -1:
			return -1;
			break;

		case 1:
			return 1;
			break;

		default:
			return (motor_double_remaining_steps[motor_index] / 2);
			break;
	}

	return 0;
}

/**
 * @brief true if a motor is moving
 * 
 * @return bool 
 */
bool Motors_isMoving()
{
	return (motor_it_enabled[0] && motor_it_enabled[1] && motor_it_enabled[2]);
}

/**
 * @brief Initialize all 3 motors.
 * 
 */
void Motors_Init()
{
	for (int i = 0; i < 3; ++i)
	{
		motor_states[i].direction = DIR_FORWARD;
		motor_states[i].enabled = false;
		motor_states[i].step_period_us = 0;

		// IT Not handled by update()
		motor_it_enabled[i] = false
		HAL_TIM_Base_Stop_IT(MOT_TIM_HANDLE_PTR(i));
	}

	Motors_Update();
}

/**
 * @brief Set the speed of 1 motor (does not update hardware)
 * 
 * @param motor_index index of the motor
 * @param direction direction for the motor
 * @param step_period_us period of a step in microseconds
 */
void Motors_SetSpeedSingle(uint8_t motor_index, MotorDirection_t direction, uint32_t step_period_us, int64_t step_count)
{
	motor_states[motor_index].direction = direction;
	motor_states[motor_index].step_period_us = step_period_us;
	motor_states[motor_index].enabled = true;

	// Too many steps is infinite steps
	if (step_count > (INT64_MAX / 2))
	{
		step_count = -1;
	}
	motor_double_remaining_steps[motor_index] = 2 * step_count;

	// Activate IT if needed
	if (!motor_it_enabled[motor_index] && step_period_us != 0)
	{
		motor_it_enabled[motor_index] = true;
		HAL_TIM_Base_Start_IT(MOT_TIM_HANDLE_PTR(motor_index));
	}
}

/**
 * @brief Stop 1 motor 
 * 
 * @param motor_index 
 */
void Motors_StopSingle(uint8_t motor_index)
{
	motor_states[motor_index].enabled = false;

	// Deactivate IT if needed
	if (motor_it_enabled[motor_index])
	{
		motor_it_enabled[motor_index] = false;
		HAL_TIM_Base_Stop_IT(MOT_TIM_HANDLE_PTR(motor_index));
	}
}

/**
 * @brief set the speed of all motors
 * 
 * @param directions array (size : 3) containing the direction of the motors
 * @param step_periods_us array (size : 3) containing the step periods of the motors
 * @param step_counts array (size : 3) containing the step counts of the motors
 */
void Motors_SetSpeedAll(MotorDirection_t *directions, uint32_t *step_periods_us, int64_t *step_counts)
{
	for (int i = 0; i < 3; ++i)
	{
		Motors_SetSpeedSingle(i, directions[i], step_periods_us[i], step_counts[i]);
	}
	Motors_Update();
}

/**
 * @brief stop all motors
 * 
 */
void Motors_StopAll()
{
	for (int i = 0; i < 3; ++i)
	{
		Motors_StopSingle(i);
	}
	Motors_Update();
}

//////////// Local functions and interrupts //////////////

/**
 * @brief Update the motors timer overflow and dir pins to match the contents of the motor_states array
 */
void Motors_Update()
{
	for (int i = 0; i < 3; ++i)
	{
		// 2 * MOT_TIM_TICK_PERIOD_US because 1 step = 2 overflows
		uint32_t arr_value = motor_states[i].step_period_us / (2 * MOT_TIM_TICKTIME_US);

		// force it to be 16 bit
		if (arr_value > 65535)
		{
			arr_value = 65535;
		}

		__HAL_TIM_SET_AUTORELOAD(MOT_TIM_HANDLE_PTR(i), arr_value);

		// Direction pin handling
		HAL_GPIO_WritePin(MOT_DIR_GPIO_PORT(i), MOT_DIR_GPIO_PIN(i), motor_states[i].direction);

		// Enable pin handling
		// Enable not supported yet.
	}
}

/**
 * @brief Timer Callback for motor handling (PWM generation). To be inserted in HAL_TIM_PeriodElapsedCallback.
 * 
 * @param htim 
 */
void Motors_TimerITCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == MOT0_TIM_HANDLE_PTR->Instance)
	{
		if (motor_double_remaining_steps[0])
		{
			HAL_GPIO_TogglePin(MOT0_STP_GPIO_Port, MOT0_STP_Pin);
			motor_double_remaining_steps[0]--;
		}
		else
		{
			HAL_GPIO_WritePin(MOT0_STP_GPIO_Port, MOT0_STP_Pin, RESET);
			motor_it_enabled[0] = false;
			HAL_TIM_Base_Stop_IT(MOT_TIM_HANDLE_PTR(0));
		}
	}
	else if (htim->Instance == MOT1_TIM_HANDLE_PTR->Instance)
	{
		if (motor_double_remaining_steps[1])
		{
			HAL_GPIO_TogglePin(MOT1_STP_GPIO_Port, MOT1_STP_Pin);
			motor_double_remaining_steps[1]--;
		}
		else
		{
			HAL_GPIO_WritePin(MOT1_STP_GPIO_Port, MOT1_STP_Pin, RESET);
			motor_it_enabled[1] = false;
			HAL_TIM_Base_Stop_IT(MOT_TIM_HANDLE_PTR(1));
		}
	}
	else if (htim->Instance == MOT2_TIM_HANDLE_PTR->Instance)
	{
		if (motor_double_remaining_steps[2])
		{
			HAL_GPIO_TogglePin(MOT2_STP_GPIO_Port, MOT2_STP_Pin);
			if (motor_double_remaining_steps[2] > 0) // only decrement if positive
			{
				motor_double_remaining_steps[2]--;
			}
		}
		else
		{
			HAL_GPIO_WritePin(MOT2_STP_GPIO_Port, MOT2_STP_Pin, RESET);
			motor_it_enabled[2] = false;
			HAL_TIM_Base_Stop_IT(MOT_TIM_HANDLE_PTR(2));
		}
	}
}
