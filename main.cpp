#include <iostream>

#include "electronic_load_tests.h"

int main() {

    using namespace std;

    // +-------------------
    // | Variables
    // +-------------------
    // RS-232 Configuration
    int n;
    int cport_nr=7,        //{"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                           // "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                           // "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                           // "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16"};
        bdrate=9600;       // 9600 baud

    unsigned char buf[4096];

    char mode[]={'8','n','1',0}; //{data, parity, stop, }

    bool menu = true;
    int choice;

    while(menu){
        printf("Select what you want to do:\n");
        printf("1 - Connection test\n");
        printf("2 - Battery Discharge test\n");
        printf("9 - Exit\n");
        printf(">> ");
        cin >> choice;

        switch(choice){
            case 1:
                n = check_connection(cport_nr, bdrate, mode);
                menu = false;
                break;
            case 2:
                n = battery_discharge(cport_nr, bdrate, mode);
                menu = false;
                break;
            case 9:
                menu = false;
                break;
            default:
                printf("Invalid option\n");
                break;
        }

        if(n == 1){
            printf("ATTENTION: check your RS232 port!\n");
            return 0;
        }
    }


    return(0);
}