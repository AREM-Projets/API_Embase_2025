/*
 * CommandUtils.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#ifndef INC_COMMANDUTILS_H_
#define INC_COMMANDUTILS_H_

#define UART_TX_MAX_LENGTH 256

#define COMMAND_DESTINATION_MAX_LENGTH 16
#define COMMAND_TYPE_MAX_LENGTH 16
#define COMMAND_NAME_MAX_LENGTH 16

#define COMMAND_MAX_ARGS 3
#define COMMAND_MAX_ARG_LENGTH 32

#define COMMAND_MAX_LENGTH 64

typedef struct
{
	Commands_Error_t err_status;

	char destination[COMMAND_DESTINATION_MAX_LENGTH];
	char type[COMMAND_TYPE_MAX_LENGTH];
	char name[COMMAND_NAME_MAX_LENGTH];

	char args[COMMAND_MAX_ARGS][COMMAND_MAX_ARG_LENGTH];
} Command_t;

typedef enum
{
	COMMAND_OK = 0,
	COMMAND_INVALID_DESTINATION,
	COMMAND_INVALID_TYPE,
	COMMAND_INVALID_NAME,
	COMMAND_INVALID_ARGS,
	COMMAND_TOO_LONG,
	COMMAND_COULD_NOT_PARSE,
	COMMAND_UNKNOWN_ERROR
} Commands_Error_t;

Commands_Error_t Utils_stringToInt32(char *src, uint32_t length, int32_t *dest);
Commands_Error_t Utils_int32ToString(int32_t src, char *dest, uint32_t length);
int32_t Utils_power(int32_t base, uint32_t exponent);
bool Utils_isNumeric(char c);
void Utils_printToUart2(char* string);

void Utils_printCommandError(Commands_Error_t error);

#endif /* INC_COMMANDUTILS_H_ */
