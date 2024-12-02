/*
 * Movement.h
 *
 *  Created on: Dec 2, 2024
 *      Author: Lenovo
 */

#ifndef INC_MOVEMENT_H_
#define INC_MOVEMENT_H_

#include "Motors.h"

typedef struct
{
	double x_mm;
	double y_mm;
	double theta_deg;
} Position_t;

void Movement_Init();
void Movement_setPosition(Position_t pos);

Position_t Movement_getPosition();
double Movement_getX();
double Movement_getY();
double Movement_getTheta();

void Movement_MoveRelativePos(Position_t relative_pos);
void Movement_MoveRelativeCoords(double dx, double dy, double dtheta);

void Movement_GotoPos(Position_t pos);
void Movement_GotoCoords(double x, double y, double theta);

#endif /* INC_MOVEMENT_H_ */
