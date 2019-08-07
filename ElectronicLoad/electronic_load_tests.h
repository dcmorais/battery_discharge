//
// Created by dcmor on 11/03/2019.
//

#ifndef BATTERY_DISCHARGE_ELETRONIC_LOAD_TESTS_H
#define BATTERY_DISCHARGE_ELETRONIC_LOAD_TESTS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "electronic_load.h"
#include <sstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

int battery_discharge(int, int, const char *);
int check_connection(int, int, const char *);
int pulse_curr(int, int, const char *);
int pulse_curr_array(int, int, const char *);
int do_tests(int, int, const char *);

#endif //BATTERY_DISCHARGE_ELETRONIC_LOAD_TESTS_H
