/*
 * GPS_navigation.hpp
 *
 *  Created on: Dec 7, 2016
 *      Author: Gautam
 */
#include "_can_dbc/generated_can.h"
#include<stdio.h>
START_CMD_t bluetooth_start={0};
GEO_SEND_HD_BR_t Nav_info={0,0};
SENSOR_SONAR_t sensor_can_msg = { near_f,near_fr,near_fl,near_b };
MOTOR_CONTROL_t motor_can_msg = { straight,stop };
unsigned char input;
unsigned char cState1=0;
unsigned char cState2=0;
unsigned char cState3=0;
bool reverse_check=false;
int upcount=0;
int turn_direction;
int turn_angle=0;


typedef enum{
   ST_REVERSE,
   ST_GPS_NAVIGATE,
   ST_OBSTACLE_AVOID
} state_t;


typedef struct{
   state_t state;
   state_t (*func)(void);
} stateTrans_t;

 state_t GPS_navigate();
 state_t obstacleAvoid();
 state_t moveReverse();
void sensorInput(SENSOR_SONAR_t sensor_can_msg);

stateTrans_t stateTrans[]=
{
		   {ST_REVERSE,moveReverse},
		   {ST_GPS_NAVIGATE,GPS_navigate},
		   {ST_OBSTACLE_AVOID,obstacleAvoid}
};



struct State
{
	speed_E out1;
	steer_E out2;
	unsigned char next_near[8];
	unsigned char next_mid[8];
};
typedef struct State StateType;
state_t cState=ST_OBSTACLE_AVOID;

StateType FSM_reverse[2]=
{
		{reverse,left,{0,0,0,0,1,1,1,1}},
		{reverse,right,{0,0,0,0,1,1,1,1}},
};



StateType FSM_obstacleAvoidance[6]=
{
		{stop,straight,{1,2,2,2,3,1,3,3},{1,4,4,2,5,1,3,3}},
		{slow,straight,{1,2,2,2,3,1,3,0},{1,4,4,2,5,1,3,3}},
		{slow,left,{1,2,3,2,3,1,3,0},{1,5,2,2,3,1,3,2}},
		{slow,right,{1,2,2,2,3,1,3,0},{1,4,3,2,5,1,3,3}},
		{slow,sleft,{1,2,2,2,3,1,3,0},{1,5,2,2,3,1,3,2}},
		{slow,sright,{1,2,2,2,3,1,3,0},{1,4,3,2,5,1,3,3}},
};

StateType FSM_GPS[6]=
{
		{stop,straight,{1,2,3,4,5}},
		{slow,straight,{1,3,3,4,4}},
		{slow,left,{1,2,3,4,4}},
		{slow,sleft,{1,2,3,4,4}},
		{slow,sright,{1,2,3,4,5}},
		{slow,right,{1,2,3,4,5}},
};

state_t moveReverse()
{

    cState2=1;
    cState3=1;

	if(upcount==40)
	{
		reverse_check=true;
		upcount=0;
	}

	if (sensor_can_msg.Sensor_Sonar_Front == danger_f)
	{
		if(reverse_check==false)
		{
			motor_can_msg.speed=brake;
			motor_can_msg.steer=straight;
			upcount++;
		}
		else if(reverse_check==true)
		{
			LE.toggle(3);
			cState1=FSM_reverse[cState1].next_near[input];
			motor_can_msg.speed=FSM_reverse[cState1].out1;
			motor_can_msg.steer=FSM_reverse[cState1].out2;
		}
	}

	else if (sensor_can_msg.Sensor_Sonar_Front != danger_f)
			{
				reverse_check=false;
				return ST_OBSTACLE_AVOID;
			}

	  return ST_REVERSE;
}

state_t GPS_navigate()
{
    upcount = 0;

	cState1=0;
	cState2=1;

    LE.toggle(2);

	if((turn_angle>=350 && turn_angle<360)||(turn_angle>=1 && turn_angle<10))
	{
		turn_direction=straight;
	}
	else if(turn_angle>=180 && turn_angle<315)
	{
		turn_direction=left;
	}
	else if(turn_angle>=315 && turn_angle<350)
	{
		turn_direction=sleft;
	}
	else if(turn_angle>10 && turn_angle<=45)
	{
		turn_direction=sright;
	}

	else if(turn_angle>45 && turn_angle<180)
	{
		turn_direction=right;
	}

    cState3=FSM_GPS[cState3].next_near[turn_direction];
	motor_can_msg.speed=FSM_GPS[cState3].out1;
	motor_can_msg.steer=FSM_GPS[cState3].out2;

	if(sensor_can_msg.Sensor_Sonar_Front==far_f&&sensor_can_msg.Sensor_Sonar_FrontLeft==far_fl&&sensor_can_msg.Sensor_Sonar_FrontRight==far_fr)
	{
		return ST_GPS_NAVIGATE;
	}
	else if(sensor_can_msg.Sensor_Sonar_Front==danger_f)
	{
	    reverse_check = false;
		return ST_REVERSE;
	}
	return ST_OBSTACLE_AVOID;
}

state_t obstacleAvoid()
{
        LE.toggle(1);

        upcount = 0;

	    cState1=0;
	    cState3=1;


		if(sensor_can_msg.Sensor_Sonar_Front==danger_f)
		{
		    reverse_check=false;
		    return ST_REVERSE;
		}
		else if(sensor_can_msg.Sensor_Sonar_Front==near_f||sensor_can_msg.Sensor_Sonar_FrontLeft==near_fl||sensor_can_msg.Sensor_Sonar_FrontRight==near_fr)
		{
            cState2=FSM_obstacleAvoidance[cState2].next_near[input];
            motor_can_msg.speed=FSM_obstacleAvoidance[cState2].out1;
            motor_can_msg.steer=FSM_obstacleAvoidance[cState2].out2;

            return ST_OBSTACLE_AVOID;
		}

		else if(sensor_can_msg.Sensor_Sonar_Front==mid_f||sensor_can_msg.Sensor_Sonar_FrontLeft==mid_fl||sensor_can_msg.Sensor_Sonar_FrontRight==mid_fr)
		{
            cState2=FSM_obstacleAvoidance[cState2].next_mid[input];
            motor_can_msg.speed=FSM_obstacleAvoidance[cState2].out1;
            motor_can_msg.steer=FSM_obstacleAvoidance[cState2].out2;
            return ST_OBSTACLE_AVOID;
		}

		else
		{
		    return ST_GPS_NAVIGATE;
		}
}

void sensorInput(SENSOR_SONAR_t sensor_can_receive)
{
	if(sensor_can_receive.Sensor_Sonar_Front == near_f || sensor_can_receive.Sensor_Sonar_FrontRight == near_fr || sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl)
	{
		if ( sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl && sensor_can_receive.Sensor_Sonar_Front == near_f&&sensor_can_receive.Sensor_Sonar_FrontRight == near_fr)
		{
			input=7;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl && sensor_can_receive.Sensor_Sonar_Front == near_f&&sensor_can_receive.Sensor_Sonar_FrontRight != near_fr)
		{
			input=6;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl && sensor_can_receive.Sensor_Sonar_Front != near_f&&sensor_can_receive.Sensor_Sonar_FrontRight == near_fr)
		{
			input=5;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == near_fl && sensor_can_receive.Sensor_Sonar_Front != near_f&&sensor_can_receive.Sensor_Sonar_FrontRight != near_fr)
		{
			input=4;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft != near_fl && sensor_can_receive.Sensor_Sonar_Front == near_f&&sensor_can_receive.Sensor_Sonar_FrontRight == near_fr)
		{
			input=3;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft != near_fl && sensor_can_receive.Sensor_Sonar_Front == near_f&&sensor_can_receive.Sensor_Sonar_FrontRight != near_fr)
		{
			input=2;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft != near_fl && sensor_can_receive.Sensor_Sonar_Front != near_f&&sensor_can_receive.Sensor_Sonar_FrontRight == near_fr)
		{
			input=1;
		}
		else
		{
			input=0;
		}
	}
	else if ( sensor_can_receive.Sensor_Sonar_Front == mid_f || sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr || sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl)
	{
		if ( sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl && sensor_can_receive.Sensor_Sonar_Front == mid_f&&sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr)
		{
			input=7;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl && sensor_can_receive.Sensor_Sonar_Front == mid_f&&sensor_can_receive.Sensor_Sonar_FrontRight !=mid_fr)
		{
			input=6;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl && sensor_can_receive.Sensor_Sonar_Front != mid_f&&sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr)
		{
			input=5;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft == mid_fl && sensor_can_receive.Sensor_Sonar_Front != mid_f&&sensor_can_receive.Sensor_Sonar_FrontRight != mid_fr)
		{
			input=4;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft != mid_fl && sensor_can_receive.Sensor_Sonar_Front == mid_f&&sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr)
		{
			input=3;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft !=mid_fl && sensor_can_receive.Sensor_Sonar_Front == mid_f&&sensor_can_receive.Sensor_Sonar_FrontRight !=mid_fr)
		{
			input=2;
		}
		else if ( sensor_can_receive.Sensor_Sonar_FrontLeft != mid_fl && sensor_can_receive.Sensor_Sonar_Front != mid_f&&sensor_can_receive.Sensor_Sonar_FrontRight == mid_fr)
		{
			input=1;
		}
		else
		{
			input=0;
		}
	}

}

bool CAR_state()
{

	sensorInput(sensor_can_msg);

	turn_angle=Nav_info.Geo_bearing - Nav_info.Geo_heading;

	if(turn_angle<0)
	{
		turn_angle=turn_angle+360;
	}

	LD.setNumber(turn_angle/10);


    cState= (stateTrans[cState].func)();

    return true;

}

