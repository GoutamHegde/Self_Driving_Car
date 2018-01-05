/*
 * motor_ctrl.hpp
 *
 *  Created on 		: Oct 15, 2016
 *      Author 		: Jaswanth
 */

#ifndef L5_APPLICATION_MOTOR_CTRL_HPP_
#define L5_APPLICATION_MOTOR_CTRL_HPP_

#include "stdint.h"
#include "lpc_pwm.hpp"
#include "singleton_template.hpp"
#include "_can_dbc/generated_can.h"

class MotorCtrl : public SingletonTemplate<MotorCtrl>
{
	public:
		MotorCtrl() : steerServo(PWM::pwm1),driveDc(PWM::pwm2)
		{
			//Constructor
		}
		void set_servo(float value) {steerServo.set(value);}
		void set_dc(float value){driveDc.set(value);}
		friend class SingletonTemplate<MotorCtrl>;

	private:
		PWM steerServo;
		PWM driveDc;

};

#define MotorCtrl MotorCtrl::getInstance()

void motor_init();
void motor_test(void);
void drive(steer_E steer,speed_E speed);
#endif /* L5_APPLICATION_MOTOR_CTRL_HPP_ */
