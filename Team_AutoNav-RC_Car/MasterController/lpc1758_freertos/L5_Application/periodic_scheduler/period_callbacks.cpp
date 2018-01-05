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

#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "string.h"

// CAN Header
#include "can.h"


//Obstacle avoidance header
#include "Motor_Sensor/Motor_Sensor.hpp"



// Sensor Message MIA Handling
const uint32_t        SENSOR_SONAR__MIA_MS = 1000;
const SENSOR_SONAR_t  SENSOR_SONAR__MIA_MSG = { near_f,near_fr,near_fl,near_b };
const uint32_t        START_CMD__MIA_MS = 1000;
const START_CMD_t     START_CMD__MIA_MSG = { 0 };



/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

/// Called once before the RTOS is started, this is a good place to initialize things once




bool period_init(void)
{
  // TODO : Shangming : Single static function to init CAN bus
    CAN_init(can1, 100, 5, 5, NULL, NULL);
    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);
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
  // TODO: Shangming: Use single function to handle can_reset_bus
    if(CAN_is_bus_off(can1))
     {
         CAN_reset_bus(can1);
     }
//    LE.toggle(1);
}

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_send = { 0 };
    can_send.msg_id                = mid;
    can_send.frame_fields.data_len = dlc;
    memcpy(can_send.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_send, 0);  // TODO: Shangming: Better to use a if statement to check either the message 
                                        //                  send successfully or not
}


void period_10Hz(uint32_t count)
{
   dbc_encode_and_send_MOTOR_CONTROL(&motor_can_msg);
}

void period_100Hz(uint32_t count)
{
        can_msg_t can_receive;
       // Empty all of the queued, and received messages within the last 10ms (100Hz callback frequency)

/* TODO: Shangming: Is it neccessary to use while loop for receiving message? 
                    If everyone send CAN message at 100Hz, then you can use
                    if(CAN_rx()) to recieve       */

        while (CAN_rx(can1, &can_receive, 0))
       {
           // Form the message header from the metadata of the arriving message
           dbc_msg_hdr_t can_msg_hdr;
           can_msg_hdr.dlc = can_receive.frame_fields.data_len;
           can_msg_hdr.mid = can_receive.msg_id;

           if (dbc_decode_START_CMD(&bluetooth_start, can_receive.data.bytes, &can_msg_hdr))
           {
               // Do Nothing
               LE.off(3);
           }

           if(dbc_decode_MOTOR_RPS(&rps_can_msg,can_receive.data.bytes, &can_msg_hdr))
           {
               //Do Nothing
           }

           // Attempt to decode the message (brute force, but should use switch/case with MID)
           else if (dbc_decode_SENSOR_SONAR(&sensor_can_msg, can_receive.data.bytes, &can_msg_hdr))
           {
//             if(bluetooth_start.BLUETOOTH_START_CMD_signal)
//               {
                   if( obstacleAvoid(sensor_can_msg))
                   {
                       LE.toggle(4);
                   }
//               }
//
//               else
//               {
//                   motor_can_msg = { straight,stop };
//               }

           }

       }


      if( dbc_handle_mia_SENSOR_SONAR(&sensor_can_msg, 10))
      {
          LE.on(2);
      }

      if( dbc_handle_mia_START_CMD(&bluetooth_start,10))
      {
          LE.on(3);
      }


}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{

}
