//
// Created by dcmor on 11/03/2019.
//

#include "electronic_load_tests.h"

int battery_discharge(int cport_nr, int bdrate, const char *mode){
    // Variables to configure the DC charge
    char   aux[512];              // Help to send complex word into the command
    double Eodv            = 10.5;// End of discharge voltage for single cell
    double Discharge_at    = .05; // Constant current discharge rate in amperes
    double Battery_volt    = 11.55;// Battery volt returns from DC charge
    double Battery_curr;          // Battery current messured

    // Timing variables
    high_resolution_clock::time_point start_clk, end_clk;
    duration<double>                  cpu_time;

    //Define Eletronic Load
    electronic_load electronic_load(cport_nr);

    // Writing into a file
    ofstream file;

    // +-----------------------
    // | Opening the COM port
    // +-----------------------
    printf("Opening the COM port\n");
    if(open_comport(cport_nr, bdrate, mode))
    {
        printf("can not open comport\n");

        return(1);
    }

    // printf("Enable RST\n");
    // RS232_enableRTS(cport_nr);

    // +--------------------------
    // | Configure the DC charge
    // +--------------------------
    printf("Configure the DC charge\n");
    // Selects Chan 1; Disables input
    printf("- Selects Chan 1 and Disables input\n");
    electronic_load.select_chan(1);
    electronic_load.disable_inputs();

    // Sets CC mode
    printf("- Sets CC mode\n");
    electronic_load.set_cc_mode();

    // Sets the CC level
    printf("- Sets the curr level\n");
    printf("-- Type the current: ");
    std::cin >> Discharge_at;
    electronic_load.set_curr_level(Discharge_at);

    // Enables the input
    printf("- Enables input\n");
    electronic_load.enable_inputs();

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
        Battery_volt = electronic_load.measure_battery_volt();

        // Measure the current
        printf("- Measure the current\n");
        Battery_curr = electronic_load.measure_battery_curr();

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
    electronic_load.disable_inputs();

    file.close();

    return 0;
}

int check_connection(int cport_nr, int bdrate, const char *mode){
    // Variables to configure the DC charge
    double Battery_volt; // Battery volt returns from DC charge
    double Battery_curr; // Battery current messured

    //Define Eletronic Load
    electronic_load electronic_load(cport_nr);

    // +-----------------------
    // | Opening the COM port
    // +-----------------------
    printf("Opening the COM port\n");
    if(open_comport(cport_nr, bdrate, mode))
    {
        printf("can not open comport\n");

        return(1);
    }

    // printf("Enable RST\n");
    // RS232_enableRTS(cport_nr);

    // +--------------------------
    // | Configure the DC charge
    // +--------------------------
    printf("Configure the DC charge\n");
    // Selects Chan 1; Disables input
    printf("- Disables input\n");
    electronic_load.disable_inputs();

    // Measure the battery voltage
    printf("- Measure the battery voltage\n");
    Battery_volt = electronic_load.measure_battery_volt();
    printf("-- Total cell voltage: %f\n",  Battery_volt);

    // Measure the current
    printf("- Measure the current\n");
    Battery_curr = electronic_load.measure_battery_curr();

    //Print
    printf("-- Actual current: %f\n", Battery_curr);

    return 0;
}

int pulse_curr(int cport_nr, int bdrate, const char *mode){
    // Variables to configure the DC charge
    char   aux[512];              // Help to send complex word into the command
    double Eodv            = 10.5;// End of discharge voltage for single cell
    double Discharge_at    = .05; // Constant current discharge rate in amperes
    double Battery_volt    = 11.55;// Battery volt returns from DC charge
    double Battery_curr;          // Battery current messured

    // Timing variables
    high_resolution_clock::time_point start_clk, end_clk;
    duration<double>                  cpu_time;

    //Define Eletronic Load
    electronic_load electronic_load(cport_nr);

    // Writing into a file
    ofstream file;

    // +-----------------------
    // | Opening the COM port
    // +-----------------------
    printf("Opening the COM port\n");
    if(open_comport(cport_nr, bdrate, mode))
    {
        printf("can not open comport\n");

        return(1);
    }

    // printf("Enable RST\n");
    // RS232_enableRTS(cport_nr);

    // +--------------------------
    // | Configure the DC charge
    // +--------------------------
    printf("Configure the DC charge\n");
    // Selects Chan 1; Disables input
    printf("- Selects Chan 1 and Disables input\n");
    electronic_load.select_chan(1);
    electronic_load.disable_inputs();

    // Sets CC mode
    printf("- Sets CC mode\n");
    electronic_load.set_cc_mode();

    // Sets the CC level
    printf("- Sets the curr level\n");
    printf("-- Type the current: ");
    std::cin >> Discharge_at;
    electronic_load.set_curr_level(Discharge_at);

    // Enables the input
    printf("- Enables input\n");
    electronic_load.enable_inputs();

    // +--------------------------
    // | Start Test
    // +--------------------------
    // Starts test routine that continuously measures and reads
    // back the voltage and current until batteries are completely discharged

    // Open file
    sprintf(aux, "pulse_curr_%f.txt", Discharge_at);
    file.open(aux, ofstream::out);

    printf("Start test\n");
    start_clk = high_resolution_clock::now();
    while(Battery_volt>Eodv) {
        // Measure the battery voltage
        printf("- Measure the battery voltage\n");
        Battery_volt = electronic_load.measure_battery_volt();

        // Measure the current
        printf("- Measure the current\n");
        Battery_curr = electronic_load.measure_battery_curr();

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
    }

    printf("- Disables input\n");
    electronic_load.disable_inputs();

    file.close();

    return 0;
}
