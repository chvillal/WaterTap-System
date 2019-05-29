//
//  kraken.h
//  eFLIP
//
//  Created by Christopher Villalpando on 5/25/19.
//  Copyright © 2019 Christopher Villalpando. All rights reserved.
//

#ifndef KRAKEN_H
#define KRAKEN_H

#include <Arduino.h>
#include <stdint.h>
#include <string>

//volatile uint8_t pinState;
//volatile uint8_t lastPinState;
//unsigned long current_t;
//unsigned long flowStart;
//unsigned long eventDuration;
//unsigned long pinChange;
//unsigned long lastPinChange;
//unsigned long pinDelta;
//unsigned long nextPinCheck;
//unsigned long nextSerialUpdate;
//uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; *
//uint8_t msg[RH_RF95_MAX_MESSAGE_LEN] = "SENSOR PACKET" ; //PLACEHOLDER ***
//uint8_t buf_len; *
//uint8_t msg_len = 14; *
//uint8_t sendPacket; *
//float volume;
//float flowRate;
//float hertz;
//bool flowing;
//bool eventStart;
//bool eventEnd; 
//char logstr[100]; *
//int  packet_len; *
//String logStr;
//char *tempPacket; *
//int s; *

class WaterSensor {
private:
    volatile uint8_t pinState;
    volatile uint8_t lastPinState;
    volatile uint8_t sensorPin;
    volatile uint8_t ledPin;
    unsigned long nextPinCheck;
    unsigned long flowStart;
    unsigned long lastPinChange;
    unsigned long pinChange;
    unsigned long pinDelta;
    unsigned long eventDuration;
    unsigned long nextSerialUpdate; 
    float volume;
    float flowRate;
    float hertz;
    bool flowing;
    bool ledEnabled;
    bool eventStart;
    bool eventEnd;
    
public:
    String logStr;
    
    void init(uint8_t pin);
    void enableLED(uint8_t pin);
    void task(unsigned long time_m);
    
}

#endif /* kraken_h */
