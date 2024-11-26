/*
 * CommandsUtils.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#include "CommandUtils.h"
#include "string.h"

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

bool Utils_isNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

Commands_Error_t Utils_stringToInt32(char *src, uint32_t length, int32_t *dest)
{
	// take minimum length between given argument and strlen
	length = (length > strlen(src)) ? length : strlen(src);
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
			return COMMAND_INVALID_ARGS;
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

	return COMMAND_OK;
}

Commands_Error_t Utils_int32ToString(int32_t src, char *dest, uint32_t length)
{
	if (length == 0 || dest == NULL)
	{
		return false;
	}

	uint32_t index = 0;
	bool isNegative = false;

	// handle negative values
	if (src < 0)
	{
		isNegative = true;
		if (index < length)
		{
			dest[index++] = '-';
		}
		else
		{
			return COMMAND_TOO_LONG;
		}
		src = -src;
	}

	// Temporary buffer to hold digits in reverse order, big enough for any int32 with 12 chars.
	char buffer[12];
	uint32_t i = 0;

	do {
		buffer[i++] = '0' + (src % 10);
		src /= 10;
	} while (src > 0);

	// If the number does not fit (should not happen)
	if (index + i >= length) {
		return COMMAND_TOO_LONG;
	}

	// Mirror buffer into dest
	while (i > 0) {
		if (index < length) {
			dest[index++] = buffer[--i];
		} else {
			return COMMAND_TOO_LONG;
		}
	}

	// Add \0 at the end
	if (index < length) {
		dest[index] = '\0';
	} else {
		return COMMAND_TOO_LONG;
	}

	return COMMAND_OK;
}

void Utils_printToUart2(char* string)
{
	HAL_UART_Transmit_DMA(&huart2, string, strlen(string));
}

void Utils_printCommandError(Commands_Error_t error)
{
	switch (error) {
		case COMMAND_INVALID_DESTINATION:
			Utils_printToUart2("");
			break;
		case COMMAND_INVALID_TYPE:
			Utils_printToUart2("");
			break;
		case COMMAND_INVALID_NAME:
			Utils_printToUart2("[Error] Command does not exist\n");
			break;
		case COMMAND_INVALID_ARGS:
			Utils_printToUart2("[Error] Invalid args given\n");
			break;
		case COMMAND_TOO_LONG:
			Utils_printToUart2("[Error] Command is too long\n");
			break;
		case COMMAND_COULD_NOT_PARSE:
			Utils_printToUart2("[Error] Could not parse command\n");
			break;
		case COMMAND_UNKNOWN_ERROR:
			Utils_printToUart2("[Error] Unspecified error\n");
			break;

		default:
			Utils_printToUart2("[Error] no error message \n");
			break;
	}
}
