//
// Created by dcmor on 11/03/2019.
//

#include "electronic_load.h"

electronic_load::electronic_load(int port) {
    cport_nr = port;
}

void electronic_load::select_chan(int chan){
    char   command[512]; // Sends the command to the Electronic Load
    char   aux[512];     // Help to send complex word into the command

    sprintf(aux, "CHAN %d\n", chan);
    strcpy(command, aux);
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
}

void electronic_load::enable_inputs() {
    char   command[512]; // Sends the command to the Electronic Load

    strcpy(command, "INPUT ON\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
}

void electronic_load::disable_inputs() {
    char   command[512]; // Sends the command to the Electronic Load

    strcpy(command, "INPUT OFF\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
}

void electronic_load::set_cc_mode() {
    char   command[512]; // Sends the command to the Electronic Load

    strcpy(command, "FUNCTION CURRENT\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
}

void electronic_load::set_curr_level(double Discharge_at) {
    char   command[512]; // Sends the command to the Electronic Load
    char   aux[512];     // Help to send complex word into the command

    sprintf(aux, "CURRENT:LEVEL %f\n", Discharge_at);
    strcpy(command, aux);
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
}

double electronic_load::measure_battery_volt() {
    char   command[512]; // Sends the command to the Electronic Load
    double Battery_volt; // Battery volt returns from Electronic Load

    unsigned char buf[4096];

    // Measure the battery voltage
    printf("- Measure the battery voltage\n");
    strcpy(command, "MEASURE:VOLTAGE?\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command

    // Get answer  from Electronic Load
    Battery_volt = read_rs232(cport_nr, buf, 4095);

    return Battery_volt;
}

double electronic_load::measure_battery_curr() {
    char   command[512]; // Sends the command to the Electronic Load
    double Battery_curr; // Battery current measured

    unsigned char buf[4096];
    strcpy(command, "MEASURE:CURRENT?\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command

    // Get answer  from Electronic Load
    Battery_curr = read_rs232(cport_nr, buf, 4095);
}