/*
 * uLCD32PTU.h
 *
 *  Created on: Nov 26, 2016
 *      Author: Jaswa
 */

#ifndef L5_APPLICATION_ULCD32PTU_H_
#define L5_APPLICATION_ULCD32PTU_H_


#include "uart3.hpp"

Uart3& uLCD = Uart3::getInstance();

void uart_init();
void display();
void send_cmd(char a,char b,char c, char d,char e);
void get_touch();

typedef enum{
    home,
    sensor,
    motor,
	geo,
}home_screen_t;

#endif /* L5_APPLICATION_ULCD32PTU_H_ */
