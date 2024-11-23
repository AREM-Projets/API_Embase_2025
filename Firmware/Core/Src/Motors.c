#include "Motors.h"

/////// Local variables //////

// Tracks the state of the motors
static MotorState_t motor_states[3];

// Tracks which motor has an enabled IT
bool motor_it_enabled[3] = {0};

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

//////// Local functions declaration //////

void Motors_Update();

/////// Functions definitions //////


void Motors_Init()
{
	for (int i = 0; i < 3; ++i)
	{
		motor_states[i].direction = DIR_FORWARD;
		motor_states[i].enabled = false;
		motor_states[i].step_period_us = 0;

		// IT Not handled by update()
		HAL_TIM_Base_Stop_IT(MOT_TIM_HANDLE_PTR(i));
	}

	Motors_Update();
}

void Motors_SetSpeedSingle(uint8_t motor_index, MotorDirection_t direction, uint32_t step_period_us)
{
	motor_states[motor_index].direction = direction;
	motor_states[motor_index].step_period_us = step_period_us;
	motor_states[motor_index].enabled = true;

	// Activate IT if needed
	if (!motor_it_enabled[motor_index] && step_period_us != 0)
	{
		motor_it_enabled[motor_index] = true;
		HAL_TIM_Base_Start_IT(MOT_TIM_HANDLE_PTR(motor_index));
	}
}

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

void Motors_SetSpeedAll(MotorDirection_t *directions, uint32_t *step_periods_us)
{
	for (int i = 0; i < 3; ++i)
	{
		Motors_SetSpeedSingle(i, directions[i], step_periods_us[i]);
	}
	Motors_Update();
}

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
		// Timer overflow //
		if (motor_states[i].step_period_us >= MOTORS_MIN_PERIOD_US && motor_states[i].step_period_us <= MOTORS_MAX_PERIOD_US)
		{
			__HAL_TIM_SET_AUTORELOAD(MOT_TIM_HANDLE_PTR(i), (uint32_t)(motor_states[i].step_period_us));
		}
		else if (motor_states[i].step_period_us > MOTORS_MAX_PERIOD_US)
		{
			__HAL_TIM_SET_AUTORELOAD(MOT_TIM_HANDLE_PTR(i), (uint32_t)(MOTORS_MAX_PERIOD_US));
		}
		else if (motor_states[i].step_period_us < MOTORS_MIN_PERIOD_US)
		{
			__HAL_TIM_SET_AUTORELOAD(MOT_TIM_HANDLE_PTR(i), (uint32_t)(MOTORS_MIN_PERIOD_US));
		}

		// Direction pin state
		HAL_GPIO_WritePin(MOT_DIR_GPIO_PORT(i), MOT_DIR_GPIO_PIN(i), motor_states[i].direction);
	}
}

void Motors_TimerITCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == MOT0_TIM_HANDLE_PTR->Instance)
	{
		HAL_GPIO_TogglePin(MOT0_STP_GPIO_Port, MOT0_STP_Pin);
	}
	else if (htim->Instance == MOT1_TIM_HANDLE_PTR->Instance)
	{
		HAL_GPIO_TogglePin(MOT1_STP_GPIO_Port, MOT1_STP_Pin);
	}
	else if (htim->Instance == MOT2_TIM_HANDLE_PTR->Instance)
	{
		HAL_GPIO_TogglePin(MOT2_STP_GPIO_Port, MOT2_STP_Pin);
	}
}
