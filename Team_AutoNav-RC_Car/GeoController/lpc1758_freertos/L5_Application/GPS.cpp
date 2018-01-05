/*
 * GPS.cpp
 *
 *  Created on: 28-Oct-2016
 *      Author: Arpita, Veena
 */

#include "examples/examples.hpp"
#include "uart2.hpp"
#include <stdio.h>
#include <String.h>
#include <GPS.hpp>
#include "tasks.hpp"
#include "can.h"
#include "_can_dbc/generated_can.h"
#include <stdlib.h>
#include "io.hpp"

char data[100];
char temp_str[100];
char *temp;
char *token[20];

/* TODO: Shangming: Better to use an empty line to separate variables and statements */

void parse_data(void) {
	int i = 0;
	const char s[2] = ",";
	strcpy(temp_str, data);
	temp = strtok(temp_str, s);
	while (temp != NULL && i <= 20) {
		token[i++] = temp;
		temp = strtok(NULL, s);
	}
}

Uart2 &u2 = Uart2::getInstance();

bool init_GPS(void) {
	u2.init(9600, 32, 64);
	char setBaudRate[] = "$PMTK251,9600*27<CR><LF>";
	char updateFreq[] = "$PMTK220,100*2F<CR><LF>";
	char setNMEA[] = "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29<CR><LF>";
	u2.putline(setBaudRate, portMAX_DELAY);
	u2.putline(updateFreq, portMAX_DELAY);
	u2.putline(setNMEA, portMAX_DELAY);
	return true;
}

bool run_GPS(void) {
	u2.gets(data, sizeof(data), portMAX_DELAY); //receiving GGA NMEA message over uart2
	parse_data();
	return true;
}

bool sendGPS_on_can() {
	GEO_SEND_LOCATION_t cmd = { 0 };
	cmd.Geo_latitude = atof(token[2]);
	cmd.Geo_longitude = atof(token[4]);
	can_msg_t can_msg = { 0 };
	dbc_msg_hdr_t msg_hdr = dbc_encode_GEO_SEND_LOCATION(can_msg.data.bytes, &cmd);
	can_msg.msg_id = msg_hdr.mid;
	can_msg.frame_fields.data_len = msg_hdr.dlc;
	CAN_tx(can1, &can_msg, 0);
	return true;
}






