/*
This was a simple project to get familiar with robot framework.
Serial port receives MNEA-meassages (example: $GPGGA,134732.000,5540.3244,N,233491231.2941,E)
and parses them to part by comma. The parser then outputs a int number
If error occurs it gives out the part of the message that has the error
like 1-6. If all is well it outputs 0. This program was uploaded to
FRDM-K64F board and then was tested with Robot Framework. The testing
script is in serial.robot file.
*/

#include "mbed.h"
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

char command[100];
int command_cnt = 0;
bool new_command = false;
char *token = NULL;
char delim[] = ",";
int from_parser = 0;
bool MNEA_msg = false;
float timestamp = 0;
float latitude_N = NULL;
int north_found = 1;
float longitude_E = NULL;
int east_found = 1;

Mutex serial_mutex;

static UnbufferedSerial pc(USBTX, USBRX);


//Interrupt from the UART
void serial_rx_int() {

    char c = 0;
    if (pc.read(&c,1)) {
        command[command_cnt] = c;
        command_cnt++;
        if (c == '\r') {
            new_command = true;
        }
    }
}

int parser(char *gps_str){
    int count = 0;
    double temp = 0;
    token = strtok(gps_str, delim);

    while(token != NULL){
        switch(count){
            case 0:
                if(strncmp(token,"$GPGGA",6) != 0){
                    return 1;
                }
                break;
            case 1:
                if(std::stod(token) > 0){
                    timestamp = std::stof(token);
                }else{
                    return 2;
                }
                break;
            case 2:
                temp = std::stod(token) / 100;
                if(temp >= 0 && temp <=90){
                    latitude_N = std::stof(token);
                }else{
                    return 3;
                }
                break;
            case 3:
                north_found = strncmp(token, "N", 1);
                if(north_found != 0){
                    return 4;
                }
                break;
            case 4:
                temp = std::stod(token) / 100;
                if(temp >= 0 && temp<= 180){
                    longitude_E = std::stof(token);
                }else{
                    return 5;
                }
                break;
            case 5:
                east_found = strncmp(token,"E",1);
                if(east_found != 0){
                    return 6;
                }
                break;
        }
        count += 1;
        token = strtok(NULL, delim);     
    }
    return 0;
}




int main(){
    pc.format(8,SerialBase::None,1);
    pc.attach(serial_rx_int,SerialBase::RxIrq);

    while(true){
        ThisThread::sleep_for(100ms);
        if(new_command == true){
            char float_str[100];
            from_parser = parser(command);
            sprintf(float_str,"%d",from_parser);
            pc.write(float_str,strlen(float_str));
            new_command = false;
            command_cnt = 0;
            MNEA_msg = false;
        }
    }
}
