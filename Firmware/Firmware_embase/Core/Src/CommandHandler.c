/*
 * CommandHandler.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#include "CommandHandler.h"

#include "string.h"

///////////////// Variables //////////////////

char command_buffer[COMMAND_MAX_LENGTH];
char uart_transmit_buffer[UART_TX_MAX_LENGTH];

Command_t current_command;

////////////////// prototypes ///////////////////

// Functions named local_... are not in the .h and only used in this file.

////////////////// Utils //////////////////////////


/////////////////// Functions ///////////////////////

void Commands_RxCallback(UART_HandleTypeDef * huart)
{
	// static variable for the current index in command_buffer (only set to 0 on startup)
	static uint32_t index = 0;

	// char that was just received
	char received_char = command_buffer[index];

	if(received_char == '\n' || received_char == '\r')
	{
		Commands_parseIntoCurrent();
		Commands_executeCurrent();
	}

	HAL_UART_Receive_IT(&huart2, &(command_buffer[index]), 1);
}

Commands_Error_t Commands_executeCurrent()
{

}

Commands_Error_t Commands_parseIntoCurrent()
{

}



