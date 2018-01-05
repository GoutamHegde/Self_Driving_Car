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

#include "tasks.hpp"
#include "examples/examples.hpp"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include<can.h>					// TODO: Shangming: Need spacing
#include<stdio.h>				// TODO: Shangming: Need spacing
#include<gpio.hpp>				// TODO: Shangming: Need spacing
#include<eint.h>				// TODO: Shangming: Need spacing
#include "utilities.h"
#include <lpc_timers.h>
#include "io.hpp"
#include<periodic_scheduler/periodic_callback.h>				// TODO: Shangming: Need spacing

#include "_can_dbc/generated_can.h"

#include <lpc_timers.h>

/* TODO: Shangming: Better to create a sensor_sonar.cpp and sensor_sonar.hpp
					file to declare and define all Sensor_Sonar related functions */

double time_start;
GPIO trigger_front = GPIO(P2_0);
GPIO trigger_frontright = GPIO(P2_1);
GPIO trigger_frontleft = GPIO(P2_2);
SENSOR_SONAR_t Sensor_cmd={near_f,near_fr,near_fl,near_b};				// TODO: Shangming: Need spacing


Sensor_Sonar_Front_E calculate_range_front(double distance)
{
	if(distance<150)
	{
		return near_f;
	}
	else if(distance<250)
	{
		return mid_f;
	}
	else
	{
		return far_f;
	}
}
Sensor_Sonar_FrontRight_E calculate_range_frontRight(double distance)
{
	if(distance<100)
	{
		return near_fr;
	}
	else if(distance<200)
	{
		return mid_fr;
	}
	else
	{
		return far_fr;
	}
}
Sensor_Sonar_FrontLeft_E calculate_range_frontLeft(double distance)
{
	if(distance<100)
	{
		return near_fl;
	}
	else if(distance<200)
	{
		return mid_fl;
	}
	else
	{
		return far_fl;
	}
}

void start_trigger(GPIO trigger)
{
	trigger.setAsOutput();
	trigger.setHigh();
	delay_us(5);
	trigger.setLow();
}

void callback_frontSend()
{

trigger_front.setLow();
time_start=lpc_timer_get_value(lpc_timer0);
trigger_front.setAsInput();
}
void callback_frontReceive()
{
	trigger_front.setLow();
	double distance_front,time_end,time;
	time_end=lpc_timer_get_value(lpc_timer0);
	time=time_end-time_start;
	distance_front=time/58;
	Sensor_cmd.Sensor_Sonar_Front=calculate_range_front(distance_front);

}

void callback_frontrightSend()
{

trigger_frontright.setLow();

time_start=lpc_timer_get_value(lpc_timer0);

}
void callback_frontrightReceive()
{

	trigger_frontright.setLow();
	double distance_frontright,time_end,time;
	time_end=lpc_timer_get_value(lpc_timer0);
	time=time_end-time_start;
	distance_frontright=time/58;
	Sensor_cmd.Sensor_Sonar_FrontRight=calculate_range_frontRight(distance_frontright);
}

void callback_frontleftSend()
{

trigger_frontleft.setLow();
 time_start=lpc_timer_get_value(lpc_timer0);

}
void callback_frontleftReceive()
{

	trigger_frontleft.setLow();
	double distance_frontleft,time_end,time;
	time_end=lpc_timer_get_value(lpc_timer0);
	time=time_end-time_start;
	distance_frontleft=time/58;
	Sensor_cmd.Sensor_Sonar_FrontLeft=calculate_range_frontLeft(distance_frontleft);

}


const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);



bool period_init(void)
{

// 	TODO: Shangming: Better to put all init statement into a single function
	
	CAN_init(can1, 100, 100, 100,0, 0);
    CAN_reset_bus(can1);
    lpc_timer_enable(lpc_timer0,1);
    lpc_timer_set_value(lpc_timer0,0);
	trigger_front.setAsOutput();
	eint3_enable_port2(0,eint_rising_edge,callback_frontSend);
	eint3_enable_port2(0,eint_falling_edge,callback_frontReceive);
	trigger_frontright.setAsOutput();
	eint3_enable_port2(1,eint_rising_edge,callback_frontrightSend);
	eint3_enable_port2(1,eint_falling_edge,callback_frontrightReceive);
	trigger_frontleft.setAsOutput();
	eint3_enable_port2(2,eint_rising_edge,callback_frontleftSend);
	eint3_enable_port2(2,eint_falling_edge,callback_frontleftReceive);

    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}


/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */

void period_1Hz(uint32_t count)
{
if(CAN_is_bus_off(can1))
	{
		CAN_reset_bus(can1);
	}
}


void sensor_pulse()
{
	start_trigger(trigger_front);
	delay_ms(20);
	trigger_front.setAsOutput();
	trigger_front.setLow();
	start_trigger(trigger_frontright);
    delay_ms(20);
	trigger_frontright.setAsOutput();
	trigger_frontright.setLow();
	start_trigger(trigger_frontleft);
    delay_ms(20);
	trigger_frontleft.setAsOutput();
	trigger_frontleft.setLow();

}




void period_10Hz(uint32_t count)
{
	        sensor_pulse();			// TODO: Shangming: Will it be better to setup the sensor in init function? Otherwise
	        						// the sensor will be initialize every 100ms

		    can_msg_t can_msg = {0};

		    // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
		    dbc_msg_hdr_t msg_hdr = dbc_encode_SENSOR_SONAR(can_msg.data.bytes, &Sensor_cmd);
		    can_msg.msg_id = msg_hdr.mid;
		    can_msg.frame_fields.data_len = msg_hdr.dlc;

		    // Queue the CAN message to be sent out
		    CAN_tx(can1, &can_msg, 0);					// TODO: Shangming: Use if statement to check either 
		    											// the message sent successful or not

}

void period_100Hz(uint32_t count)
{

}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
}
