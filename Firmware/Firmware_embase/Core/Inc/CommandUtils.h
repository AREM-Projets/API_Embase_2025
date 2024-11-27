/*
 * CommandUtils.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#ifndef INC_COMMANDUTILS_H_
#define INC_COMMANDUTILS_H_

#include "stdint.h"
#include "stdbool.h"

#define UART_TX_MAX_LENGTH 256

#define COMMAND_DESTINATION_MAX_LENGTH 16
#define COMMAND_TYPE_MAX_LENGTH 16
#define COMMAND_NAME_MAX_LENGTH 16

#define COMMAND_MAX_ARGS 3
#define COMMAND_ARG_MAX_LENGTH 32

#define COMMAND_MAX_LENGTH 64

typedef enum
{
	CMD_ERROR_OK = 0,
	CMD_ERROR_INVALID_DESTINATION,
	CMD_ERROR_INVALID_TYPE,
	CMD_ERROR_INVALID_NAME,
	CMD_ERROR_INVALID_ARGS,
	CMD_ERROR_COMMAND_TOO_LONG,
	CMD_ERROR_COULD_NOT_PARSE,
	CMD_ERROR_UNKNOWN,
	CMD_ERROR_NOT_IMPLEMENTED
} Commands_Error_t;

typedef struct
{
	Commands_Error_t err_status;

	uint8_t destination[COMMAND_DESTINATION_MAX_LENGTH];
	uint8_t type[COMMAND_TYPE_MAX_LENGTH];
	uint8_t name[COMMAND_NAME_MAX_LENGTH];

	uint8_t args[COMMAND_MAX_ARGS][COMMAND_ARG_MAX_LENGTH];
} Command_t;

void Utils_initEmptyCommand(Command_t *command_ptr);
uint8_t *Utils_getCommandInfoString(Command_t command, uint8_t *buffer);

Commands_Error_t Utils_setCommandPartByIndex(Command_t *command_ptr, uint32_t index, uint8_t *str);
Commands_Error_t Utils_stringToInt32(uint8_t *src, uint32_t length, int32_t *dest);
Commands_Error_t Utils_int32ToString(int32_t src, uint8_t *dest, uint32_t length);
int32_t Utils_power(int32_t base, uint32_t exponent);
bool Utils_isNumeric(uint8_t c);
void Utils_printToUart2(uint8_t* string);

void Utils_printCommandError(Commands_Error_t error);

#endif /* INC_COMMANDUTILS_H_ */
