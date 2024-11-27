/*
 * CommandsRobot.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#ifndef INC_COMMANDSROBOT_H_
#define INC_COMMANDSROBOT_H_

#include "CommandUtils.h"

Commands_Error_t Robot_get(uint8_t *var_name);
Commands_Error_t Robot_set(uint8_t *var_name, uint8_t *value);

#endif /* INC_COMMANDSROBOT_H_ */
