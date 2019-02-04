#include <iostream>
#include <fstream>
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
    printf("-- Send command: %s\n", command);
    RS232_cputs(cport_nr, command);
#ifdef _WIN32
    Sleep(1000);
#else
    usleep(1000000);  /* sleep for 1 Second */
#endif
};

float read_rs232(int cport_nr, unsigned char *buf, int size) {
    int n;

    printf("- Reading data from DC Charge\n");
    n = RS232_PollComport(cport_nr, buf, size);

    if (n > 0) {
        buf[n] = 0;   /* always put a "null" at the end of a string! */
    }
    else if (n == 0) {
        printf("ERROR: Received no value from DC charge!\n");
        buf = (unsigned char *) "0";
    }
    std::cout << buf << std::endl;

#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);  /* sleep for 100 milliSeconds */
#endif
    return std::stof((char *)buf);
}

int main() {
    // +-------------------
    // | Variables
    // +-------------------
    // RS-232 Configuration
    int n,
            cport_nr=7,        /* /dev/ttys0 (com1 on windows) */
                               /*{"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                                  "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                                  "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                                  "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16"};*/
            bdrate=9600;       /* 9600 baud */

    unsigned char buf[4096];

    char mode[]={'8','n','1',0}; //{data, parity, stop, }

    // Variables to configure the DC charge
    char   command[512];          // Sends the command to the DC charge
    char   aux[512];              // Help to send complex word into the command
    double Eodv            = 8.4; // End of discharge voltage for single cell
    int    Number_of_cells = 3;   // Number of cells to be discharged in series
    double Discharge_at    = .05; // Constant current discharge rate in amperes
    float  Battery_volt    = 12;  // Battery volt returns from DC charge
    float  Battery_curr;          // Battery current messured 

    // Timing variables
    clock_t start, end;
    double cpu_time;

    // Writing into a file
    std::ofstream file;

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
    strcpy(command, "CHAN 1;:INPUT OFF");
    send_rs232(cport_nr, command);

    // Sets CC mode
    printf("- Sets CC mode\n");
    strcpy(command, "FUNCTION CURRENT");
    send_rs232(cport_nr, command);

    // Sets the CC level
    printf("- Sets the curr level\n");
    printf("-- Type the current: ");
    std::cin >> Discharge_at;
    sprintf(aux, "CURRENT:LEVEL %f", Discharge_at);
    strcpy(command, aux);
    send_rs232(cport_nr, command);

    // Enables the input
    printf("- Enables input\n");
    strcpy(command, "INPUT ON");
    send_rs232(cport_nr, command);

    // +--------------------------
    // | Start Test
    // +--------------------------
    // Starts test routine that continuously measures and reads
    // back the voltage and current until batteries are completely discharged
    
    // Open file
    sprintf(aux, "save_curr_%f.txt", Discharge_at);
    file.open(aux, std::ofstream::out);

    printf("Start test\n");
    start = clock();
    while(Battery_volt>Eodv) {
        // Measure the battery voltage
        printf("- Measure the battery voltage\n");
        strcpy(command, "MEASURE:VOLTAGE?");
        send_rs232(cport_nr, command);

        // Get answer  from DC Charge
        Battery_volt = read_rs232(cport_nr, buf, 4095);
        printf("- Total cell voltage: %f\n",  Battery_volt);
        //Battery_volt = std::stof((char *)buf);


        // Measure the current
        printf("- Measure the current\n");
        strcpy(command, "MEASURE:CURRENT?");
        send_rs232(cport_nr, command);

        // Get answer  from DC Charge
        Battery_curr = read_rs232(cport_nr, buf, 4095);
        printf("- Actual current: %f\n", Battery_curr);
        //Battery_curr = std::stof((char *)buf);

        // Computed spend time
        end = clock();
        cpu_time = ((double) (end-start)) / CLOCKS_PER_SEC;
        printf("- Used time: %f\n", cpu_time);

        // Save received data into a file
        file << Battery_curr << " " << cpu_time << std::endl;
    }

    file.close();
}