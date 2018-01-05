/*
 * motor_ctrl.cpp
 *
 *  Created on 		: Oct 15, 2016
 *      Author 		: Jaswanth
 *      Version		: 0.2(Testing DC and SERVO motor)
 *      PWM 		: pwm1 initialized @ 50Hz	/	20ms
 *      Wiring		: Orange-PWM; Red-VCC; Brown-Ground(SERVO)
 *      			: White-PWM; Red- NC	; Black-Ground (DC)
 */
#include "motor_ctrl.hpp"
#include "utilities.h"
#include <stdio.h>
//#include "can.h"


#define HARD_LEFT            9.5
#define S_LEFT               8.5

#define STRAIGHT             7.5

#define S_RIGHT              6.5
#define HARD_RIGHT           5.5

#define STOP                 7.5

#define SLOW                 7.63
#define NORMAL               7.68
#define FAST                 7.73

/* 1 - 123 2
 * 2 - 12345
 * 3 - 1234 2
 * 4 - 123456 2
 * 5 - 1234 2
 * 6 - 1234 4
 * 7 - 12345 4
 * 8 - 12345 4
 * 9 - 123 2
 * 10- 12 1
 */
int get_Servopwm(steer_E enum_val)
{
	switch (enum_val)
    {
	    case straight	:
	        			return STRAIGHT*100;
        case left 		:
        				return HARD_LEFT*100;
        case sleft 		:
        				return S_LEFT*100;
        case right 		:
        				return HARD_RIGHT*100;
        case sright 	:
        				return S_RIGHT*100;
        default			:
        				return STRAIGHT*100;
    }

}


float get_DCpwm(speed_E enum_val)
{
	switch (enum_val)
    {
	    case stop	:
	        			return STOP*100;
        case slow 		:
        				return SLOW*100;
        case normal 	:
        				return NORMAL*100;
        case fast 		:
        				return FAST*100;
        default			:
        				return STOP*100;
    }

}


void motor_init()
{
	drive(straight,stop);
}

/* TODO: Shangming: Spacing on all statements */

void drive(steer_E steer,speed_E speed)
{

	static int servoVal,servo_Currentval=STRAIGHT*100;
	servoVal = get_Servopwm(steer);
	//MotorCtrl.set_servo(servoVal);
	printf("steer:%d\n",servoVal);
	while(servoVal!=servo_Currentval)
		{	printf("current:%d\n",servo_Currentval);
			if(servo_Currentval>servoVal)
			{
				servo_Currentval-=5;
				MotorCtrl.set_servo(servo_Currentval*0.01);
				delay_ms(5);
			}
			else if(servo_Currentval<servoVal)
			{
				servo_Currentval+=5;
				MotorCtrl.set_servo(servo_Currentval*0.01);
				delay_ms(5);
			}
		}
		if(servoVal==servo_Currentval)
		{
			MotorCtrl.set_servo(servo_Currentval*0.01);
			delay_ms(5);
		}


	static int dcVal,dc_Currentval=STOP*100;
	dcVal = get_DCpwm(speed);
	printf("speed:%d\n",dcVal);
	while(dcVal!=dc_Currentval)
	{
		printf("speed:%d\n",dc_Currentval);
		if(dc_Currentval>dcVal)
		{
			dc_Currentval-=1;
			MotorCtrl.set_dc(dc_Currentval*0.01);
			delay_ms(50);
		}
		else if(dc_Currentval<dcVal)
		{
			dc_Currentval+=1;
			MotorCtrl.set_dc(dc_Currentval*0.01);
			delay_ms(50);
		}
	}
	if(dcVal==dc_Currentval)
	{
		MotorCtrl.set_dc(dc_Currentval*0.01);
		delay_ms(50);
	}

}

