/*
 * CommandsConfig.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#include "CommandsConfig.h"
#include "string.h"

Team_t team = TEAM_NONE;
Strategy_t strategy = STRATEGY_NONE;

////// Prototypes //////////

Commands_Error_t Config_get(char *var_name);
Commands_Error_t Config_set(char *var_name, char *value);

Team_t local_stringToTeamEnum(char *string);
Strategy_t loacl_stringToStrategyEnum(char *string);

///////// Local utils functions ///////////

Team_t local_stringToTeamEnum(char *string)
{
	if ((strcmp(string, "blue") == 0) || (strcmp(string, "1") == 0))
	{
		return TEAM_BLUE;
	}
	else if ((strcmp(string, "yellow") == 0) || (strcmp(string, "2") == 0))
	{
		return TEAM_YELLOW;
	}

	return TEAM_NONE;
}

Strategy_t loacl_stringToStrategyEnum(char *string)
{
	if ((strcmp(string, "basic") == 0) || (strcmp(string, "1") == 0))
	{
		return STRATEGY_BASIC;
	}
	else if ((strcmp(string, "test") == 0) || (strcmp(string, "2") == 0))
	{
		return STRATEGY_TEST;
	}

	return STRATEGY_NONE;
}

///////// Functions /////////

Commands_Error_t Config_get(char *var_name)
{
	if (strcmp(var_name, "team") != 0)
	{
		return COMMAND_INVALID_NAME;
	}
	else
	{
		switch (team) {
			case TEAM_BLUE:
				Utils_printToUart2("blue");
				return COMMAND_OK;
				break;

			case TEAM_YELLOW:
				Utils_printToUart2("yellow");
				return COMMAND_OK;
			default:
				Utils_printToUart2("none");
				return COMMAND_OK;
				break;
		}
	}
}

Commands_Error_t Config_set(char *var_name, char *value)
{

}

Commands_Error_t Config_handleCommand(Command_t command)
{
	// Verify destination
	if (strcmp(command.destination, "config") != 0)
	{
		return COMMAND_UNKNOWN_ERROR;
	}

	// Handle command type
	if (strcmp(command.type, "get") == 0)
	{
		return Config_get(command.args[0]);
	}
	else if (strcmp(command.type, "set") == 0)
	{
		return Config_set(command.args[0], command.args[1]);
	}
	else
	{
		return COMMAND_INVALID_NAME;
	}

}
