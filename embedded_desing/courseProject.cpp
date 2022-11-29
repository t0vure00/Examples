//Tekijät: Ahokas Pauliina ja Vuokila Reetta, TVT20SPL

#include "mbed.h"
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#define EVENT_ONE  1

EventFlags flags;

char command[100];
int command_cnt = 0;
bool new_MNEA_command = false;
bool new_MNEA_command_true = false;
bool new_DEBUG_command = false;
char *token = NULL;
char delim[] = ",";
int count = 0;
int from_parser = 0;
//float timestamp = 0;
//float latitude_N = NULL;
int north_found = 1;
//float longitude_E = NULL;
int east_found = 1;

double airQuality = 0;

typedef struct {
    double timestamp;
    double longitude;
    double latitude;
    double qualityvalue;
}data_msg_t;

Queue<data_msg_t, 10> dataValues;

MemoryPool<data_msg_t, 10> mpool;

int msg_count = 0;
double air_min = 100; 
double air_max = 0;

Queue<double, 10> airValues;

Mutex serial_mutex;

Thread t1;
Thread t2;

static UnbufferedSerial pc(USBTX, USBRX);


//Interrupt from the UART
void serial_rx_int() {

    char c = 0;
    if (pc.read(&c,1)) {
        command[command_cnt] = c;
        command_cnt++;

        if (c == '\r') {
            if(command[0] == '$'){
                new_MNEA_command = true;
            }else if(command[0] == '?'){
                new_DEBUG_command = true;
            }
        }
    }
}


double t_2_air_quality(){
    //Creating the air quality value
    double airQuality = std::rand() % 100;
    //debug("T2 %f\n", airQuality);
    return airQuality;
}

void t_3_data_aggregator(){
    //Create a data point that combines GPS info and air quality measurement
    data_msg_t *data = mpool.try_alloc();
    while(true){
        flags.wait_all(EVENT_ONE);
        double value = t_2_air_quality();

        data->qualityvalue = value; //Laitetaan airQuality arvo yhteiseen jonoon
        dataValues.try_put(data);

        airValues.try_put(&value); //Laitetaan airquality omaan jonoon task 4 tarkistusta varten 
        ThisThread::sleep_for(100ms);
    }
}


void t_4_data_anlysis(){
    //Updating the air quality min and max values
    while(true){
        double *data_out;
        flags.set(EVENT_ONE);
        if (airValues.empty() == false){

            airValues.try_get(&data_out);
            if(*data_out < air_min){
                //debug("Asetetaan uusi arvo minimiin\n");
                air_min = *data_out;
            }
            if(*data_out > air_max){
                //debug("Asetetaan uusi arvo maximiin\n");
                air_max = *data_out;
            }
        }
 
        ThisThread::sleep_for(100ms);

    }
}

void debug_parser(char *debug_str){
    char float_str[100];

    token = strtok(debug_str, delim);

    while(token != NULL){
        if(strncmp(token,"?min",4) == 0){
            sprintf(float_str,"minimi: %f\n", air_min);
            debug("%s", float_str);
        }else if(strncmp(token,"?max",4) == 0){
            sprintf(float_str,"maksimi: %f\n", air_max);
            debug("%s", float_str);
        }else if(strncmp(token,"?cnt",4) == 0){
            sprintf(float_str,"msg_count: %d\n", msg_count);
            debug("%s", float_str);
        }
        token = strtok(NULL, delim);  
    }
}

int parser(char *gps_str){
    debug("PARSERISSA\n");
    int count = 0;
    double temp = 0;
    data_msg_t *data = mpool.try_alloc();   // data allocointi data struct jonoa varten kuten esimerkissä: 
                                            //https://os.mbed.com/docs/mbed-os/v6.15/apis/queue.html

    token = strtok(gps_str, delim);

    while(token != NULL){
        switch(count){
            case 0:
                    if(strncmp(token,"$GPGGA",6) == 0){
                    debug("MNEA viesti vastaanotettu %s\n", token);
                    }else{
                        debug("EI MNEA viesti");
                        return 1;
                    }
                break;
            case 1:
                if(std::stod(token) > 0){
                    data->timestamp = std::stod(token);
                    dataValues.try_put(data); //Laitetaan saatu arvo yhteiseen jonoon
                    debug("timestamp is: %s\n", token);
                }else{
                    debug("Palautetaan 2\n");
                    return 2;
                }
                break;
            case 2:
                temp = std::stod(token) / 100;
                if(temp >= 0 && temp <=90){
                    data->latitude = temp;
                    dataValues.try_put(data);
                    debug("latitude_N is: %s\n", token);
                }else{
                    debug("Palautetaan 3\n");
                    return 3;
                }
                break;
            case 3:
                north_found = strncmp(token, "N", 1);
                if(north_found == 0){
                    debug("north is found: %d\n", north_found);
                }else{
                    debug("north was not correct");
                    return 4;
                }
                break;
            case 4:
                temp = std::stod(token) / 100;
                if(temp >= 0 && temp<= 180){
                    data->longitude = temp;
                    dataValues.try_put(data);
                    debug("longitude_e is: %s\n", token);
                }else{
                    debug("longitude väärä");
                    return 5;
                }
                break;
            case 5:
                east_found = strncmp(token,"E",1);
                if(east_found == 0){
                    debug("east is found: %d\n", east_found);
                }else{
                    debug("east was not correct\n");
                    return 6;
                }
                break;
        }
        count = count + 1;
        token = strtok(NULL, delim);     
    }
    return 0;
}


void for_parser_thread(){
    debug("for_parser_thread\n");
    from_parser = parser(command);
    debug("Mainissa saatiin: %d\n", from_parser);
    if(from_parser == 0){
        msg_count++;
        new_MNEA_command_true = true;
    }
}


int main(){
    pc.format(8,SerialBase::None,1);
    pc.attach(serial_rx_int,SerialBase::RxIrq);

    t1.start(t_4_data_anlysis);
    t2.start(t_3_data_aggregator);

    while(true){
        ThisThread::sleep_for(100ms);
        if(new_MNEA_command == true){
            //debug("Saatiin uusi MNEA viesti %s\n", command);
            for_parser_thread();
            new_MNEA_command = false;
            command_cnt = 0;
        }

        if(new_DEBUG_command == true){
            //debug("Saatiin uusi DEBUG komento %s\n", command);
            debug_parser(command);
            new_DEBUG_command = false;
            command_cnt = 0;
        }
    }
}