/*
 * CommandsConfig.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#include "CommandsConfig.h"
#include "string.h"

///////// variables /////////

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
			return (uint8_t *) "blue\n";
			break;

		case TEAM_YELLOW:
			return (uint8_t *) "yellow\n";
			break;

		default:
			return (uint8_t *) "none\n";
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
			return (uint8_t *) "basic\n";
			break;

		case STRATEGY_TEST:
			return (uint8_t *) "test\n";
			break;

		default:
			return (uint8_t *) "none\n";
			break;
	}
}

///////// Functions /////////

/**
 * @brief execute a get command
 * 
 * @param var_name variable to get the value of
 * @return Commands_Error_t 
 */
Commands_Error_t Config_get(uint8_t *var_name)
{
	if (strcmp((char *) var_name, "team") == 0)
	{
		printToUart(COMMAND_UART_HANDLE_PTR, local_teamEnumToString(team));
		return CMD_ERROR_OK;
	}
	else if(strcmp((char *) var_name, "strategy") == 0)
	{
		printToUart(COMMAND_UART_HANDLE_PTR, local_strategyEnumToString(strategy));
		return CMD_ERROR_OK;
	}
	else
	{
		return CMD_ERROR_INVALID_ARGS;
	}
}

/**
 * @brief execute a set command 
 * 
 * @param var_name variable to be set
 * @param value new value to set the variable to
 * @return Commands_Error_t 
 */
Commands_Error_t Config_set(uint8_t *var_name, uint8_t *value)
{
	if (strcmp((char *) var_name, "team") == 0)
	{
		team = local_stringToTeamEnum(value);

		printToUart(COMMAND_UART_HANDLE_PTR, local_teamEnumToString(team));
		return CMD_ERROR_OK;
	}
	else if(strcmp((char *) var_name, "strategy") == 0)
	{
		strategy = local_stringToStrategyEnum(value);

		printToUart(COMMAND_UART_HANDLE_PTR, local_strategyEnumToString(strategy));
		return CMD_ERROR_OK;
	}
	else
	{
		return CMD_ERROR_INVALID_ARGS;
	}
}

/**
 * @brief Handle a command for the Config destination
 * 
 * @param command command to be handled
 * @return Commands_Error_t 
 */
Commands_Error_t Config_handleCommand(Command_t command)
{
	// Verify destination
	if (strcmp((char *) command.destination, "config") != 0)
	{
		return CMD_ERROR_UNKNOWN;
	}

	// Handle command type
	if (strcmp((char *) command.name, "get") == 0)
	{
		return Config_get(command.args[0]);
	}
	else if (strcmp((char *) command.name, "set") == 0)
	{
		return Config_set(command.args[0], command.args[1]);
	}
	else
	{
		return CMD_ERROR_INVALID_NAME;
	}

}
