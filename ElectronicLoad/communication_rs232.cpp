//
// Created by dcmor on 11/03/2019.
//

#include "communication_rs232.h"

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

int open_comport(int cport_nr, int bdrate, const char *mode){
    return RS232_OpenComport(cport_nr, bdrate, mode);
}