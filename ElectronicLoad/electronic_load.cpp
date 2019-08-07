//
// Created by dcmor on 11/03/2019.
//

#include "electronic_load.h"

electronic_load::electronic_load(int port) {
    cport_nr = port;
    cnt = 0;
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
    /*
    if(cnt < 20) {
        Battery_curr = 1.0;
    } else{
        Battery_curr = 0.0;
    }*/
    return Battery_curr;
}

void electronic_load::measure_battery_volt_array(ofstream &file) {
    char   command[512]; // Sends the command to the Electronic Load
    double Battery_volt[150]; // Battery volt returns from Electronic Load
    double received_r232;
    int size = 8192;
    int ii = 0;
    int k = 0;

    unsigned char buf[size];
    unsigned char received[size];

    //clean data
    memset(command, 0, sizeof(command)); //clean command
    memset(buf, 0, sizeof(buf)); //clean command
    memset(received, 0, sizeof(received)); //clean command

    // Measure the battery voltage
    strcpy(command, "MEASURE:ARRAY:VOLTAGE?\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
    //strcpy(command, "SENSe:SWEep:TINTerval 100E-3;POINts 20\n");
    strcpy(command, "SENSe:SWEep:POINts 20\n");
    memset(buf, 0, sizeof(buf)); //clean command
    memset(received, 0, sizeof(received)); //clean command

    usleep(600000);  /* sleep for 600 milliSeconds */
    // Get answer  from Electronic Load
   // for(int i=0;i<66;i++) {
        received_r232 = read_rs232(cport_nr, buf, size - 1);
        for (int j = 0; j < size; j++) {
            if (buf[j] != 0) {
                //std::cout << buf[j] << std::endl;
                file << buf[j];
                if (buf[j] != 44) {
                    received[k] = buf[j];
                    k++;
                } else {
                    file << endl;
                   // Battery_volt[ii] = std::stof((char *) received);
                    k = 0;
                   // std::cout << "ii = " << ii << " - " << Battery_volt[ii] << std::endl;

                    //ii++;
                    memset(received, 0, sizeof(received)); //clean command
                }
            }
    //    }
    }

  //  for(int j=0;j<10;j++) {
  //      std::cout << Battery_volt[j] << std::endl;
  //  }

    usleep(100000);  /* sleep for 100 milliSeconds */
//    return measure_battery_volt();
}

void electronic_load::set_samples() {
    char   command[512]; // Sends the command to the Electronic Load

    //strcpy(command, "SENSe:SWEep:POINts 20\n");
    //send_rs232(cport_nr, command);
    //memset(command, 0, sizeof(command)); //clean command
    strcpy(command, "SENSe:SWEep:TINTerval 100E-3;POINts 20\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
}

