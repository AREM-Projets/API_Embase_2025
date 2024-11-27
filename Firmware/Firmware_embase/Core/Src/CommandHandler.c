/*
 * CommandHandler.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#include "CommandHandler.h"

#include "string.h"
#include "usart.h"

///////////////// Variables //////////////////

uint8_t command_buffer[COMMAND_MAX_LENGTH];
uint8_t uart_transmit_buffer[UART_TX_MAX_LENGTH];

Command_t current_command;


////////////////// prototypes ///////////////////

// Functions named local_... are not in the .h and only used in this file.

Commands_Error_t Commands_executeCurrent();
Commands_Error_t Commands_parseIntoCurrent();


int32_t local_getCharIndex(uint8_t *str, uint32_t length, uint8_t to_find);

////////////////// Utils //////////////////////////

/**
 * @brief Get the first index of to_find in str. Returns -1 if it isn't there
 * 
 * @param str string 
 * @param length length of the string
 * @param to_find char to findin the string
 * @return int32_t 
 */
int32_t local_getCharIndex(uint8_t *str, uint32_t length, uint8_t to_find)
{
	// Update length to account for end of string
	length = (strlen((char *) str) > length) ? length : strlen((char *) str);

	for (int32_t i = 0; i < length; i++)
	{
		if(str[i] == to_find)
		{
			return i;
		}
	}

	return -1;
}


/////////////////// Functions ///////////////////////

/**
 * @brief Interrupt function to be inserted in the RxCompleteCallback.
 * 
 * @param huart 
 */
void Commands_RxCallback(UART_HandleTypeDef * huart)
{
	// static variable for the current index in command_buffer (only set to 0 on startup)
	static uint32_t index = 0;
	static Commands_Error_t err = CMD_ERROR_OK;

	if (index >= COMMAND_MAX_LENGTH)
	{
		Utils_printCommandError(CMD_ERROR_COMMAND_TOO_LONG);
		index = 0;
	}

	// uint8_t that was just received
	uint8_t received_char = command_buffer[index];
	index++;

	if(received_char == '\n' || received_char == '\r')
	{
		// debug
		// char debug_buffer[64] = {0};
		// strcpy(debug_buffer, "executing : ");
		// strcat(debug_buffer, (char *)command_buffer);
		// strcat(debug_buffer, "\n");
		// Utils_printToUart2((uint8_t *)debug_buffer);


		err = Commands_parseIntoCurrent();
		if (err)
		{
			Utils_printCommandError(err);
		}
		// err = Commands_executeCurrent();
		// if (err)
		// {
		// 	Utils_printCommandError(err);
		// }

		index = 0;
	}

	HAL_UART_Receive_IT(&huart2, &(command_buffer[index]), 1);
}

/**
 * @brief Execute the command in current_command.
 * 
 * @return Commands_Error_t 
 */
Commands_Error_t Commands_executeCurrent()
{
	Commands_Error_t err = CMD_ERROR_OK;

	if (!strcmp((char *) current_command.destination, "config"))
	{
		err = Config_handleCommand(current_command);
	}
	else
	{
		err = CMD_ERROR_INVALID_DESTINATION;
	}

	return err;
}

/**
 * @brief Parse the command received via UART into current_command
 * 
 * @return Commands_Error_t 
 */
Commands_Error_t Commands_parseIntoCurrent()
{
	Utils_initEmptyCommand(&current_command);

	// Check that the command ends (and get length)
	int32_t command_length = local_getCharIndex(command_buffer, COMMAND_MAX_LENGTH, '\n');
	// Test for \r as well just in case
	if (command_length < 0)
	{
		command_length = local_getCharIndex(command_buffer, COMMAND_MAX_LENGTH, '\r');
	}

	// Error handling
	if (command_length < 0)
	{
		if (strlen((char *) command_buffer) < COMMAND_MAX_LENGTH)
		{
			return CMD_ERROR_COULD_NOT_PARSE;
		}
		return CMD_ERROR_COMMAND_TOO_LONG;
	}

	// Copy command in a local buffer
	uint8_t temp_buffer[COMMAND_MAX_LENGTH + 1] = {0};
	strncpy((char *) temp_buffer, (char *) command_buffer, command_length); // Removes the \n at the end
	temp_buffer[command_length] = ' '; // replace the \n with a space

	// Store received command into current_command
	uint32_t command_part_index = 0;
	int32_t start_index = 0;
	while (strlen((char *) &temp_buffer[start_index]) > 0)
	{
		// Check if argument overflows max_args
		if (command_part_index >= COMMAND_MAX_ARGS + 3)
		{
			return CMD_ERROR_INVALID_ARGS;
		}

		int32_t space_index = local_getCharIndex(&temp_buffer[start_index], command_length, ' ');
		if (space_index < 0 || space_index > command_length) // > and not >= on purpose, there is a \0 on index [command_length] so is ok
		{
			return CMD_ERROR_COULD_NOT_PARSE;
		}

		Utils_setCommandPartByIndex(&current_command, command_part_index, &temp_buffer[start_index], space_index);

		start_index += space_index + 1; // deletes everything before the space by manipulating the pointer.
		command_length -= (space_index + 1); // Updates command length
		command_part_index++;
	}

	// uint8_t print_buffer[64] = {0};
	// Utils_printToUart2(Utils_getCommandInfoString(current_command, print_buffer));

	return CMD_ERROR_OK;
}



