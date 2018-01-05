#include "stdio.h"
#include "_can_dbc/generated_can.h"
#include "lpc_timers.h"

// CAN signal structures
SENSOR_SONAR_t sensor_can_msg = { near_f,near_fr,near_fl,near_b };
MOTOR_CONTROL_t motor_can_msg = { straight,stop };
MOTOR_RPS_t rps_can_msg = {equal};
START_CMD_t bluetooth_start = { 0 };

bool reverse_check = false;
static int upcount = 0;

bool obstacleAvoid (SENSOR_SONAR_t sensor_can_receive)
{

     // Handling obstacle at very Near Region
      if ( sensor_can_receive.Sensor_Sonar_Front == danger_f)
        {

              if (reverse_check == false)
              {
                  motor_can_msg.speed = brake;
                  motor_can_msg.steer = straight;

                  upcount++;
              }

              else if (reverse_check == true)
              {
                      motor_can_msg.speed = reverse;
              }


              if (upcount == 30)
              {
                  reverse_check = true;
                  upcount = 0;
              }

              // Steering change based on region of obstacle

                    if ( (sensor_can_receive.Sensor_Sonar_FrontRight == near_fr && sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl))
                    {
                       if (reverse_check == true)
                        {
                            motor_can_msg.steer = straight;
                        }

                    }

                    else if (sensor_can_receive.Sensor_Sonar_FrontRight == near_fr )
                    {
                        if (reverse_check == true)
                         {
                            motor_can_msg.steer = left;
                         }
                    }

                    else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl )
                    {
                        if (reverse_check == true)
                         {
                            motor_can_msg.steer = right;
                         }
                    }

                    else
                    {
                        if (reverse_check == true)
                         {
                            motor_can_msg.steer = straight;
                         }
                    }

        }

    // Handling obstacle at Near Region
     else if ( sensor_can_receive.Sensor_Sonar_Front == near_f || sensor_can_receive.Sensor_Sonar_FrontRight == near_fr || sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl)
    {
            reverse_check = false;
            upcount = 0;

//            // RPS speed calculation
//
//            if (rps_can_msg.Rps_Sensor != equal )
//            {
//                if (rps_can_msg.Rps_Sensor == high)
//                {
//                    motor_can_msg.speed = stop;
//                }
//
//                else if (rps_can_msg.Rps_Sensor == low)
//                {
//                    motor_can_msg.speed = normal;
//                }
//            }
//
//            else
//            {
                motor_can_msg.speed = slow;

//            }

            // Steering change based on region of obstacle

            if ( (sensor_can_receive.Sensor_Sonar_Front == near_f && sensor_can_receive.Sensor_Sonar_FrontRight == near_fr && sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl))
            {
                motor_can_msg.steer = straight;
            }

            else if ( sensor_can_receive.Sensor_Sonar_Front == near_f && sensor_can_receive.Sensor_Sonar_FrontRight == near_fr )
            {
                motor_can_msg.steer = left;
            }
            else if ( sensor_can_receive.Sensor_Sonar_Front == near_f && sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl )
            {
                motor_can_msg.steer = right;
            }

            else if ( sensor_can_receive.Sensor_Sonar_Front == near_f)
            {
                motor_can_msg.steer = right;
            }

            else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl)
            {
                motor_can_msg.steer = right;
            }
            else if ( sensor_can_receive.Sensor_Sonar_FrontRight == near_fr )
            {
                motor_can_msg.steer = left;
            }
            else if(sensor_can_receive.Sensor_Sonar_FrontRight == near_fr && sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl)
            {
                 motor_can_msg.steer = straight;
             }
    }
    // Handling obstacle at Mid Region
    else if ( sensor_can_receive.Sensor_Sonar_Front == mid_f || sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr || sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl)
    {
        reverse_check= false;
        upcount = 0;

//        // RPS speed calculation
//        if (rps_can_msg.Rps_Sensor != equal )
//        {
//            if (rps_can_msg.Rps_Sensor == high)
//            {
//                motor_can_msg.speed = stop;
//            }
//
//            else if (rps_can_msg.Rps_Sensor == low)
//            {
//                motor_can_msg.speed = normal;
//            }
//        }
//
//        else
//        {
            motor_can_msg.speed = slow;
//        }

        // Steering change based on region of obstacle

           if ( (sensor_can_receive.Sensor_Sonar_Front == mid_f && sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr && sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl))
           {
               motor_can_msg.steer = sright;
           }
            else if(sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr && sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl)
            {
                 motor_can_msg.steer = straight;
            }
            else if ( sensor_can_receive.Sensor_Sonar_Front == mid_f && sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr )
            {
                motor_can_msg.steer = sleft;
            }
            else if ( sensor_can_receive.Sensor_Sonar_Front == mid_f && sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl )
            {
                motor_can_msg.steer = sright;
            }
            else if ( sensor_can_receive.Sensor_Sonar_Front == mid_f || sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl)
            {
                motor_can_msg.steer = sright;
            }
            else if ( sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr )
            {
                motor_can_msg.steer = sleft;
            }
    }
    // No obstacle region
    else if ( sensor_can_receive.Sensor_Sonar_Front == far_f)
   {
        reverse_check = false;
        upcount = 0;

//        // RPS speed calculation
//        if (rps_can_msg.Rps_Sensor != equal )
//        {
//
//            if (rps_can_msg.Rps_Sensor == high)
//            {
//                motor_can_msg.speed = stop;
//            }
//
//            else if (rps_can_msg.Rps_Sensor == low)
//            {
//                motor_can_msg.speed = normal;
//            }
//        }
//
//        else
//        {
            motor_can_msg.speed = slow;
//        }

        // Steering change based on region of obstacle

        motor_can_msg.steer = straight;
   }
    return true;
}
