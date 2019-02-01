#include <iostream>
#include <cstring>
#include <string>
#include <ctime>

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
    double Eodv            = 8.4; // End of discharge voltage for single cell
    int   Number_of_cells = 3;   // Number of cells to be discharged in series
    double Discharge_at    = .05; // Constant current discharge rate in amperes
    float Battery_volt    = 12;  // Battery volt returns from DC charge

    // Timing variables
    clock_t start, end;
    double cpu_time;

    // +-----------------------
    // | Opening the COM port
    // +-----------------------
    printf("Opening the COM port\n");    
    if(RS232_OpenComport(cport_nr, bdrate, mode))
    {
        printf("can not open comport\n");

        return(0);
    }


    // +--------------------------
    // | Configure the DC charge
    // +--------------------------
    printf("Configure the DC charge\n");
    // Selects Chan 1; Disables input
    printf("- Selects Chan 1 and Disables input\n");
    strcpy_s(command, "CHAN 1;:INPUT OFF");
    printf("-- Send command: %s\n", command);
    send_rs232(cport_nr, command);

    // Sets CC mode
    printf("- Sets CC mode\n");
    strcpy_s(command, "FUNCTION CURRENT");
    printf("-- Send command: %s\n", command);
    send_rs232(cport_nr, command);

    // Sets the CC level
    printf("- Sets the CC level\n");
    sprintf_s(aux, "CURRENT:LEVEL %f", Discharge_at);
    strcpy_s(command, aux);
    printf("-- Send command: %s\n", command);
    send_rs232(cport_nr, command);

    // Enables the input
    printf("- Enables input\n");
    strcpy_s(command, "INPUT ON");
    printf("-- Send command: %s\n", command);
    send_rs232(cport_nr, command);

    // +--------------------------
    // | Start Test
    // +--------------------------
    // Starts test routine that continuously measures and reads
    // back the voltage and current until batteries are completely discharged
    
    printf("Start test\n");
    start = clock();
    while(Battery_volt>Eodv) {
        // Measure the battery voltage
        printf("- Measure the battery voltage\n");
        strcpy_s(command, "MEASURE:VOLTAGE?");
        printf("-- Send command: %s\n", command);
        send_rs232(cport_nr, command);

        // Get answer  from DC Charge
        printf("- Get answer  from DC Charge\n");
        n = read_rs232(cport_nr, buf, 4095);
        printf("- Total cell voltage: %s\n", (char *) buf);
        Battery_volt = std::stof((char *)buf);


        // Measure the current
        printf("- Measure the current\n");
        strcpy_s(command, "MEASURE:CURRENT?");
        printf("-- Send command: %s\n", command);
        send_rs232(cport_nr, command);

        // Get answer  from DC Charge
        printf("- Get answer  from DC Charge\n");
        n = read_rs232(cport_nr, buf, 4095);
        printf("- Actual current: %s\n", (char *) buf);

        // Computed spend time
        end = clock();
        cpu_time = ((double) (end-start)) / CLOCKS_PER_SEC;
        printf("- Used time: %f\n", cpu_time);
    }
}