/*
 * uLCD32PTU.cpp
 *
 *  Created on: Nov 26, 2016
 *      Author: Jaswa
 */
//#include "uLCD32PTU.h"
#include "uart3.hpp"
bool geo_flag = false;
bool geo_page = false;
bool sensor_flag = false;
bool sensor_page = false;
bool motor_flag = false;
bool motor_page = false;
bool home_flag = false;
bool home_page = false;

char rx_char;

//home_screen_t	current_page = home;

//extern MOTOR_CONTROL_t Motor_cmd_msg;
//extern SENSOR_SONAR_t Sensor_msg;
//extern GEO_SEND_HD_BR_t Geo_HB_msg;
//extern GEO_SEND_LOCATION_t Geo_LL_msg;
//extern SENSOR_BATTERY_t Sensor_B_msg;

char PSC[6]={0};
char ack=0;
Uart3& uLCD = Uart3::getInstance();
void uart_init()
{
	uLCD.init(115200, 100, 100);
}

void send_cmd(char a,char b,char c, char d,char e)
{
	char command[6]={a,b,c,d,e,0};
	for(int i=0; i<5; i++)
	{
		command[5]^=command[i];
	}
	for(int i=0; i<6; i++)
	{
		uLCD.putChar(command[i], 10);
	}
	uLCD.getChar(&ack,10);
}

void display()
{

}

void get_touch()
{
	uLCD.getChar(&rx_char,0);
	if (rx_char == 0x07)
	{
		PSC[0]=rx_char;
		for(int i=1;i<6;i++) {
			uLCD.getChar(PSC+i,0);
		}
	}
}
