//
// Created by dcmor on 11/03/2019.
//

#ifndef BATTERY_DISCHARGE_COMMUNICATION_RS232_H
#define BATTERY_DISCHARGE_COMMUNICATION_RS232_H

#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

void send_rs232(int cport_nr, char  command[512]);
double read_rs232(int cport_nr, unsigned char *buf, int size);
double read_array_rs232(int cport_nr, unsigned char *buf, int size);
int    open_comport(int, int, const char *);
void   close_comport(int);

#endif //BATTERY_DISCHARGE_COMMUNICATION_RS232_H
