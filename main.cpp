#include <iostream>
#include <cstring>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

void send_rs232(int cport_nr, char  command[512]){
    RS232_cputs(cport_nr, command);
#ifdef _WIN32
    Sleep(1000);
#else
    usleep(1000000);  /* sleep for 1 Second */
#endif
};

int read_rs232(int cport_nr, unsigned char *buf, int size) {
    int n;
    n = RS232_PollComport(cport_nr, buf, size);

    if (n > 0) {
        buf[n] = 0;   /* always put a "null" at the end of a string! */
    }

#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);  /* sleep for 100 milliSeconds */
#endif
    return n;
}

int main() {
    // +-------------------
    // | Variables
    // +-------------------
    // RS-232 Configuration
    int n,
            cport_nr=0,        /* /dev/ttys0 (com1 on windows) */
            bdrate=9600;       /* 9600 baud */

    unsigned char buf[4096];

    char mode[]={'8','n','1',0}; //{data, parity, stop, }

    // Variables to configure the DC charge
    char  command[512];          // Sends the command to the DC charge
    char  aux[512];              // Help to send complex word into the command
    float Eodv            = 8.4; // End of discharge voltage for single cell
    int   Number_of_cells = 3;   // Number of cells to be discharged in series
    float Discharge_at    = .05; // Constant current discharge rate in amperes
    float Battery_volt    = 12;  // Battery volt returns from DC charge

    // +-----------------------
    // | Opening the COM port
    // +-----------------------
    if(RS232_OpenComport(cport_nr, bdrate, mode))
    {
        printf("can not open comport\n");

        return(0);
    }


    // +--------------------------
    // | Configure the DC charge
    // +--------------------------
    // Selects Chan 1; Disables input
    strcpy(command, "CHAN 1;:INPUT OFF");
    send_rs232(cport_nr, command);

    // Sets CC mode
    strcpy(command, "FUNCTION CURRENT");
    send_rs232(cport_nr, command);

    // Sets the CC level
    sprintf(aux, "CURRENT:LEVEL %f", Discharge_at);
    strcpy(command, aux);
    send_rs232(cport_nr, command);

    // Enables the input
    strcpy(command, "INPUT ON");
    send_rs232(cport_nr, command);

    // +--------------------------
    // | Start Test
    // +--------------------------
    // Starts test routine that continuously measures and reads
    // back the voltage and current until batteries are completely discharged
    while(Battery_volt>Eodv) {
        // Measure the battery voltage
        strcpy(command, "MEASURE:VOLTAGE?");
        send_rs232(cport_nr, command);

        // Answer from DC Charge
        n = read_rs232(cport_nr, buf, 4095);
        printf("Total cell voltage: %s\n", (char *) buf);
        Battery_volt = std::stof((char *)buf);


        // Measure the current
        strcpy(command, "MEASURE:CURRENT?");
        send_rs232(cport_nr, command);

        // Answer from DC Charge
        n = read_rs232(cport_nr, buf, 4095);
        printf("Actual current: %s\n", (char *) buf);
    }
}