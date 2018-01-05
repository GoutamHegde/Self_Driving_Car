/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */
#include<stdio.h>
#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "can.h"
#include "_can_dbc/generated_can.h"
#include"uart3.hpp"
#include "file_logger.h"
//const uint32_t Test_MSG_Rx_MIA_MS= 3000;


/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

// TODO: Shangming: Better to create a bluetooth.cpp and bluetooth.hpp file, and 
//                  put all bluetooth related function in those files

Uart3& u3 = Uart3::getInstance();
void send_msg(char mydata);
/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    // TODO: Shangming: Use a single function to init CAN bus
	u3.init(9600, 0, 500);
	CAN_init(can1,500,100,100,0,0);
	CAN_reset_bus(can1);
    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    return true; // Must return true upon success
}


bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    return CAN_tx(can1, &can_msg, 0);           // TODO: Shangming: Better to use if statement to check either
                                                //                  the message send successfully or not
}



/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */

void period_1Hz(uint32_t count)
{
    // TODO: Shangming: Use a single function to handle CAN_reset_bus

	if(CAN_is_bus_off(can1)==1)
	{
	CAN_reset_bus(can1);
	}
	    //LE.toggle(1);
}


void period_10Hz(uint32_t count)
{
	void send_msg(char mydata);            // TODO: Shangming: Better move this declaration into your bluetooth header file
		char rx='c';
		if(u3.getChar(&rx)==true)
		{	if(rx=='c')
			{
			send_msg('0');
			}
			else{
			send_msg(rx);
			}
		}
		else
		{
			printf("Connection Lost\n");
		}
}

void period_100Hz(uint32_t count)
{
    //LE.toggle(3);
}

void period_1000Hz(uint32_t count)
{
    //LE.toggle(4);
}

void send_msg(char mydata)                  // TODO: Shangming: Better to define your function in your header file
{
can_msg_t msg={0};
START_CMD_t Bt_Start_Msg={0};
if(dbc_encode_and_send_START_CMD(&Bt_Start_Msg))
{
CAN_tx(can1, &msg, portMAX_DELAY);
}
}
