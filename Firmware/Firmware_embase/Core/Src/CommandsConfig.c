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

Commands_Error_t Config_get(uint8_t *var_name);
Commands_Error_t Config_set(uint8_t *var_name, uint8_t *value);

Team_t local_stringToTeamEnum(uint8_t *string);
Strategy_t local_stringToStrategyEnum(uint8_t *string);

///////// Local utils functions ///////////

Team_t local_stringToTeamEnum(uint8_t *string)
{
	if ((strcmp((char *) string, "blue") == 0) || (strcmp((char *) string, "1") == 0))
	{
		return TEAM_BLUE;
	}
	else if ((strcmp((char *) string, "yellow") == 0) || (strcmp((char *) string, "2") == 0))
	{
		return TEAM_YELLOW;
	}

	return TEAM_NONE;
}

uint8_t *local_teamEnumToString(Team_t team)
{
	switch (team) {

		case TEAM_BLUE:
			return (uint8_t *) "blue";
			break;

		case TEAM_YELLOW:
			return (uint8_t *) "yellow";
			break;

		default:
			return (uint8_t *) "none";
			break;
	}
}

Strategy_t local_stringToStrategyEnum(uint8_t *string)
{
	if ((strcmp((char *) string, "basic") == 0) || (strcmp((char *) string, "1") == 0))
	{
		return STRATEGY_BASIC;
	}
	else if ((strcmp((char *) string, "test") == 0) || (strcmp((char *) string, "2") == 0))
	{
		return STRATEGY_TEST;
	}

	return STRATEGY_NONE;
}

uint8_t *local_strategyEnumToString(Strategy_t strategy)
{
	switch (strategy) {
		case STRATEGY_BASIC:
			return (uint8_t *) "basic";
			break;

		case STRATEGY_TEST:
			return (uint8_t *) "test";
			break;

		default:
			return (uint8_t *) "none";
			break;
	}
}

///////// Functions /////////

Commands_Error_t Config_get(uint8_t *var_name)
{
	if (strcmp((char *) var_name, "team") == 0)
	{
		Utils_printToUart2(local_teamEnumToString(team));
		return CMD_ERROR_OK;
	}
	else if(strcmp((char *) var_name, "strategy") == 0)
	{
		Utils_printToUart2(local_strategyEnumToString(strategy));
		return CMD_ERROR_OK;
	}
	else
	{
		return CMD_ERROR_INVALID_NAME;
	}
}

Commands_Error_t Config_set(uint8_t *var_name, uint8_t *value)
{
	return CMD_ERROR_NOT_IMPLEMENTED;
}

Commands_Error_t Config_handleCommand(Command_t command)
{
	// Verify destination
	if (strcmp((char *) command.destination, "config") != 0)
	{
		return CMD_ERROR_UNKNOWN;
	}

	// Handle command type
	if (strcmp((char *) command.type, "get") == 0)
	{
		return Config_get(command.name);
	}
	else if (strcmp((char *) command.type, "set") == 0)
	{
		return Config_set(command.name, command.args[0]);
	}
	else
	{
		return CMD_ERROR_INVALID_TYPE;
	}

}
