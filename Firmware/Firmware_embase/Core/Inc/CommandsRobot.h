/*
 * CommandsRobot.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#ifndef INC_COMMANDSROBOT_H_
#define INC_COMMANDSROBOT_H_

#include "CommandUtils.h"

Commands_Error_t Robot_get(char *var_name);
Commands_Error_t Robot_set(char *var_name, char *value);

#endif /* INC_COMMANDSROBOT_H_ */
