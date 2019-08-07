//
// Created by dcmor on 11/03/2019.
//

#ifndef BATTERY_DISCHARGE_ELECTRONIC_LOAD_H
#define BATTERY_DISCHARGE_ELECTRONIC_LOAD_H

#include <iostream>
#include <fstream>
#include <cstring>

#include "communication_rs232.h"

using namespace std;

class electronic_load {
    int cport_nr;

public:
    int cnt;
    electronic_load(int );

    void select_chan(int );
    void set_cc_mode();
    void set_curr_level(double );
    void enable_inputs();
    void disable_inputs();
    void set_samples();

    double measure_battery_volt();
    double measure_battery_curr();
    void measure_battery_volt_array(ofstream &);

};


#endif //BATTERY_DISCHARGE_ELECTRONIC_LOAD_H
