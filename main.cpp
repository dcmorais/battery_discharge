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

#include "rs232.h"

void send_rs232(int cport_nr, char  command[512]){
    printf("-- Send command: %s", command);
    RS232_cputs(cport_nr, command);
#ifdef _WIN32
    Sleep(1000);
#else
    usleep(100000);  /* sleep for 100 milliSeconds */
#endif
};

double read_rs232(int cport_nr, unsigned char *buf, int size) {
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
    using namespace std;
    using namespace std::chrono;

    // +-------------------
    // | Variables
    // +-------------------
    // RS-232 Configuration
    double n;
    int cport_nr=7,        //{"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                           // "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                           // "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                           // "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16"};
        bdrate=9600;       // 9600 baud

    unsigned char buf[4096];

    char mode[]={'8','n','1',0}; //{data, parity, stop, }

    // Variables to configure the DC charge
    char   command[512];          // Sends the command to the DC charge
    char   aux[512];              // Help to send complex word into the command
    double Eodv            = 9; // End of discharge voltage for single cell
    int    Number_of_cells = 3;   // Number of cells to be discharged in series
    double Discharge_at    = .05; // Constant current discharge rate in amperes
    double Battery_volt    = 11.55;// Battery volt returns from DC charge
    double Battery_curr;          // Battery current messured

    // Timing variables
    high_resolution_clock::time_point start_clk, end_clk;
    duration<double>                  cpu_time;

    // Writing into a file
    ofstream file;

    //file << Battery_volt << std::endl;
    // +-----------------------
    // | Opening the COM port
    // +-----------------------
    printf("Opening the COM port\n");
    if(RS232_OpenComport(cport_nr, bdrate, mode))
    {
        printf("can not open comport\n");

        return(0);
    }

   // printf("Enable RST\n");
   // RS232_enableRTS(cport_nr);

    // +--------------------------
    // | Configure the DC charge
    // +--------------------------
    printf("Configure the DC charge\n");
    // Selects Chan 1; Disables input
    printf("- Selects Chan 1 and Disables input\n");
    strcpy(command, "CHAN 1;:INPUT OFF\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command

    // Sets CC mode
    printf("- Sets CC mode\n");
    strcpy(command, "FUNCTION CURRENT\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command

    // Sets the CC level
    printf("- Sets the curr level\n");
    printf("-- Type the current: ");
    std::cin >> Discharge_at;
    sprintf(aux, "CURRENT:LEVEL %f\n", Discharge_at);
    strcpy(command, aux);
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command

/*
    printf("- Measure the battery voltage\n");
    strcpy(command, "MEASURE:VOLTAGE?\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command

    // Get answer  from DC Charge
    Battery_volt = read_rs232(cport_nr, buf, 4095);
    printf("- Total cell voltage: %g\n",  Battery_volt);
            printf("- Measure the current\n");
            strcpy(command, "MEASURE:CURRENT?\n");
            send_rs232(cport_nr, command);
            memset(command, 0, sizeof(command)); //clean command

            // Get answer  from DC Charge
            Battery_curr = read_rs232(cport_nr, buf, 4095);
            printf("- Actual current: %g\n", Battery_curr);

*/

    // Enables the input
    printf("- Enables input\n");
    strcpy(command, "INPUT ON\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command
    // +--------------------------
    // | Start Test
    // +--------------------------
    // Starts test routine that continuously measures and reads
    // back the voltage and current until batteries are completely discharged
    
    // Open file
    sprintf(aux, "save_curr_%f.txt", Discharge_at);
    file.open(aux, ofstream::out);

    printf("Start test\n");
    start_clk = high_resolution_clock::now();
    while(Battery_volt>Eodv) {
        // Measure the battery voltage
        printf("- Measure the battery voltage\n");
        strcpy(command, "MEASURE:VOLTAGE?\n");
        send_rs232(cport_nr, command);
        memset(command, 0, sizeof(command)); //clean command

        // Get answer  from DC Charge
        Battery_volt = read_rs232(cport_nr, buf, 4095);
        //Battery_volt = stof((char *)buf);


        // Measure the current
        printf("- Measure the current\n");
        strcpy(command, "MEASURE:CURRENT?\n");
        send_rs232(cport_nr, command);
        memset(command, 0, sizeof(command)); //clean command

        // Get answer  from DC Charge
        Battery_curr = read_rs232(cport_nr, buf, 4095);

        //Print
        printf("- Total cell voltage: %f\n",  Battery_volt);
        printf("- Actual current: %f\n", Battery_curr);
        //Battery_curr = stof((char *)buf);

        // Computed spend time
        end_clk = high_resolution_clock::now();
        cpu_time = duration_cast<duration<double>>(end_clk-start_clk);
        printf("- Used time: %f\n", cpu_time);

        // Save received data into a fill
        memset(aux, 0, sizeof(aux));
        sprintf(aux, "%.5g %.5g %.8g\n",Battery_volt, Battery_curr, cpu_time);
        file.write(aux, 512);
#ifdef _WIN32
        Sleep(500);
#else
//      usleep(100000);  /* sleep for 100 milliSeconds */
        usleep(500000);  /* sleep for 500 milliSeconds */
#endif
    }

    printf("- Disables input\n");
    strcpy(command, ":INPUT OFF\n");
    send_rs232(cport_nr, command);
    memset(command, 0, sizeof(command)); //clean command

    file.close();

    return(0);
}