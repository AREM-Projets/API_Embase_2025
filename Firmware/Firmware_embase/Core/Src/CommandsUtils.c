/*
 * CommandsUtils.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#include "CommandUtils.h"
#include "string.h"
#include "main.h"
#include "usart.h"
#include "Uart_io.h"

Commands_Error_t Utils_PrintCommandToUart(Command_t command)
{
	char buffer[COMMAND_MAX_LENGTH] = {0};
	strcpy(buffer, (char *) command.destination);
	strcat(buffer, " ");
	strcat(buffer, (char *) command.name);
	for (uint32_t i = 0; i < COMMAND_MAX_ARGS; i++)
	{
		strcat(buffer, " ");
		strcat(buffer, (char *) command.args[i]);
	}
	strcat(buffer, "\n");

	printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) buffer);
	return CMD_ERROR_OK;
}

/**
 * @brief Initialize a command (set all elements to 0)
 * 
 * @param command_ptr 
 */
void Utils_initEmptyCommand(Command_t *command_ptr)
{
	memset(command_ptr->destination, 0, COMMAND_DESTINATION_MAX_LENGTH);
	memset(command_ptr->name, 0, COMMAND_NAME_MAX_LENGTH);
	for (uint32_t i = 0; i < COMMAND_MAX_ARGS; i++)
	{
		memset(command_ptr->args[i], 0, COMMAND_ARG_MAX_LENGTH);
	}
}

/**
 * @brief set the value of a command part (destination, name, args)
 * 
 * @param command_ptr pointer to the command
 * @param index index of the part (0: destination, 1: name, >2: args)
 * @param str string to store
 * @param length length of the string
 * @return Commands_Error_t 
 */
Commands_Error_t Utils_setCommandPartByIndex(Command_t *command_ptr, uint32_t index, uint8_t *str, uint32_t length)
{
	switch (index) {
		case 0:
			if (length > COMMAND_DESTINATION_MAX_LENGTH)
			{
				return CMD_ERROR_INVALID_DESTINATION;
			}
			strncpy((char *) command_ptr->destination, (char *) str, length);
			return CMD_ERROR_OK;
			break;

		case 1:
			if (length > COMMAND_NAME_MAX_LENGTH)
			{
				return CMD_ERROR_INVALID_NAME;
			}
			strncpy((char *) command_ptr->name, (char *) str, length);
			return CMD_ERROR_OK;
			break;

		default:
			// Check if argument can exist (index not too big)
			if ((index >= (COMMAND_MAX_ARGS + 2)) || (length > COMMAND_ARG_MAX_LENGTH))
			{
				return CMD_ERROR_INVALID_ARGS;
			}
			strncpy((char *) command_ptr->args[index - 2], (char *) str, length);
			return CMD_ERROR_OK;

			break;
	}

	return CMD_ERROR_COULD_NOT_PARSE;
}

/**
 * @brief returns base^exponent
 * 
 * @param base 
 * @param exponent 
 * @return int32_t 
 */
int32_t Utils_power(int32_t base, uint32_t exponent)
{
	int32_t res = 1;

	while (exponent > 0)
	{
		if (exponent % 2 == 1)
		{
			res *= base;
		}
		base *= base;
		exponent /= 2;
	}

	return res;
}

/**
 * @brief returns true if c is a number
 * 
 * @param c char to test
 * @return bool 
 */
bool Utils_isNumeric(uint8_t c)
{
	return (c >= '0' && c <= '9');
}

/**
 * @brief Transform a string to an int32_t and store it
 * 
 * @param src string to transform
 * @param length length of the string
 * @param dest pointer to the resulting int32_t
 * @return Commands_Error_t 
 */
Commands_Error_t Utils_stringToInt32(uint8_t *src, uint32_t length, int32_t *dest)
{
	// take minimum length between given argument and strlen
	length = (length > strlen((char *) src)) ? length : strlen((char *) src);
	int32_t res = 0;
	uint32_t start_index = 0;

	// sign handling part 1
	if (src[0] == '-')
	{
		start_index = 1;
	} else if (src[0] == '+')
	{
		start_index = 1;
	}

	for (int i = start_index; i < length; ++i)
	{
		if (!Utils_isNumeric(src[i]))
		{
			return CMD_ERROR_INVALID_ARGS;
		}

		// add each digit separately
		res += src[i] * Utils_power(10, length - i - 1);
	}


	// sign handling part 2 : the minus strikes back
	if (src[0] == '-')
	{
		*dest = -res;
	} else
	{
		*dest = res;
	}

	return CMD_ERROR_OK;
}

/**
 * @brief Transform an int32_t to a string
 * 
 * @param src int32_t to transform
 * @param dest string to store the number into
 * @param length length of the string
 * @return Commands_Error_t 
 */
Commands_Error_t Utils_int32ToString(int32_t src, uint8_t *dest, uint32_t length)
{
	if (length == 0 || dest == NULL)
	{
		return false;
	}

	uint32_t index = 0;

	// handle negative values
	if (src < 0)
	{
		if (index < length)
		{
			dest[index++] = '-';
		}
		else
		{
			return CMD_ERROR_COMMAND_TOO_LONG;
		}
		src = -src;
	}

	// Temporary buffer to hold digits in reverse order, big enough for any int32 with 12 chars.
	uint8_t buffer[12];
	uint32_t i = 0;

	do {
		buffer[i++] = '0' + (src % 10);
		src /= 10;
	} while (src > 0);

	// If the number does not fit (should not happen)
	if (index + i >= length) {
		return CMD_ERROR_COMMAND_TOO_LONG;
	}

	// Mirror buffer into dest
	while (i > 0) {
		if (index < length) {
			dest[index++] = buffer[--i];
		} else {
			return CMD_ERROR_COMMAND_TOO_LONG;
		}
	}

	// Add \0 at the end
	if (index < length) {
		dest[index] = '\0';
	} else {
		return CMD_ERROR_COMMAND_TOO_LONG;
	}

	return CMD_ERROR_OK;
}

/**
 * @brief print an error message based on the error.
 * 
 * @param error error to print the message for.
 */
void Utils_printCommandError(Commands_Error_t error)
{
	switch (error) {
		case CMD_ERROR_INVALID_DESTINATION:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] Invalid command destination\n");
			break;
		case CMD_ERROR_INVALID_NAME:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] Invalid command name\n");
			break;
		case CMD_ERROR_INVALID_ARGS:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] Invalid arguments given\n");
			break;
		case CMD_ERROR_COMMAND_TOO_LONG:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] Command is too long\n");
			break;
		case CMD_ERROR_COULD_NOT_PARSE:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] Could not parse command\n");
			break;
		case CMD_ERROR_UNKNOWN:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] Unspecified error\n");
			break;

		case CMD_ERROR_NOT_IMPLEMENTED:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] Not implemented\n");
			break;

		default:
			printToUart(COMMAND_UART_HANDLE_PTR, (uint8_t *) "[Error] no error message (default case) \n");
			break;
	}
}
