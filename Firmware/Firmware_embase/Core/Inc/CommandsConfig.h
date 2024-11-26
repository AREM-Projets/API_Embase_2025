/*
 * CommandsConfig.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#ifndef INC_COMMANDSCONFIG_H_
#define INC_COMMANDSCONFIG_H_

#include "CommandUtils.h"

typedef enum
{
	ARG_INT,
	ARG_STRING
} ArgType_t;

typedef enum
{
	char var_name[COMMAND_TYPE_MAX_LENGTH];

} ConfigVar_t;

typedef enum
{
	TEAM_NONE = 0,
	TEAM_BLUE = 1,
	TEAM_YELLOW = 2
} Team_t;

typedef enum
{
	STRATEGY_NONE = 0,
	STRATEGY_BASIC = 1,
	STRATEGY_TEST = 2
} Strategy_t;

////////// List of config variables /////////

extern Team_t team;
extern Strategy_t strategy;

////////// Functions //////////

Commands_Error_t Config_handleCommand(Command_t command);


#endif /* INC_COMMANDSCONFIG_H_ */
