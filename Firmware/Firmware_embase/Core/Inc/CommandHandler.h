/*
 * CommandHandler.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Lenovo
 */

#ifndef INC_COMMANDHANDLER_H_
#define INC_COMMANDHANDLER_H_

#include "main.h"
#include "stdbool.h"
#include "CommandUtils.h"

#include "CommandsConfig.h"

void Commands_RxCallback(UART_HandleTypeDef * huart);

#endif /* INC_COMMANDHANDLER_H_ */
