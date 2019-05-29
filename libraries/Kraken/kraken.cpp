//
//  kraken.cpp
//  eFLIP
//
//  Created by Christopher Villalpando on 5/25/19.
//  Copyright © 2019 Christopher Villalpando. All rights reserved.
//

#include "kraken.h"
#include <stdio.h>

void WaterSensor::init(uint8_t pin)
{
    //init flowmeter sensor pin
    sensorPin = pin; 
    pinMode(sensorPin, INPUT);
    digitalWrite(sensorPin, HIGH);
    
    //init sensor variables
    nextPinCheck = millis();
    lastPinState = false;
    flowing = false;
    
}

void WaterSensor::enableLED(uint8_t pin) 
{
    //init sensor led pin
    if (ledEnabled == false){
        ledPin = pin;
        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, LOW);
        ledEnabled = true;
    }
}

void WaterSensor::task(unsigned long time_m)
{
    
    // Simulate 1ms interrupt, nextpincheck is in millis
    if ( time_m < nextPinCheck) {
        return;
    }
    
    //update next sensor check timeout value
    nextPinCheck++;
    
    //read sensor pin
    pinState = digitalRead(sensorPin);
    
    //if low to high transition
    if (pinState != lastPinState && pinState == HIGH){
        
        if (flowing == false){
            flowStart = time_m;
            Serial.println("FLOW EVENT STARTED");
            lastPinChange = time_m;
            flowing = true;
            lastPinState = pinState;
            eventStart=true;
            if (ledEnabled){
                digitalWrite(ledPin, HIGH);
            }
            return;
        }
        
        pinChange = time_m;
        pinDelta = pinChange - lastPinChange; 
        
        if (pinDelta < 1000){
            hertz = 1000.0000 /pinDelta;
            flowRate = hertz/(60*7.5); // L/s
            volume += flowRate*(pinDelta/1000.0000);
        }
        
        lastPinChange = pinChange;
        
    } else if (flowing == true && pinState == lastPinState &&
               (time_m - lastPinChange) > 3000){
        flowing = false;
        eventEnd=true;
        
        eventDuration = time_m - flowStart - 3000;
        String volumestr = String("L ");
        String volumeval = String(volume, 2);
        String durationstr = String("Sec");
        String durationval = String( eventDuration/1000 );
        
        logStr = volumeval + volumestr + durationval + durationstr;
        
        volume = 0;
        Serial.println("FLOW EVENT FINISHED");
        Serial.println(logStr);
        if (ledEnabled){
            digitalWrite(ledPin, LOW);
        }
        
    }
    
    lastPinState = pinState;
}

void WaterSensor::manager_init(void){
    eventStart=false;
    eventEnd=false;
    nextSerialUpdate = millis();
    
    buf_len = sizeof(buf);
    sendPacket = 0;
}

void WaterSensor::eventManager_task(unsigned long time_m){
    
    // if flowing state active, print metrics every 1s
    if ( time_m >= nextSerialUpdate ){
        nextSerialUpdate += 1000;
        if (flowing == true){
            Serial.println("FLOW UPDATE");
            Serial.print("Flow: ");
            Serial.println(flowRate);
            Serial.print("Volume: ");
            Serial.println(volume);
            
        }
    }
    
    // if an event just started, prepare notification packet
    if ( eventStart ){
        eventStart=false;
        //    sendPacket++;
        //
        Serial.println("CRAFTING START PACKET");
        //    //insert header+payload code here
        char *payload = "EVENT-START";
        //    char *bitmap = FLIP_construct_bitmap();
        //    char *header = FLIP_construct_header();
        //
        //    //memset(packet, '\0', strlen(packet));
        //    char *packet = FLIP_construct_packet(bitmap,header,payload);
        //    packet_len = get_packet_length();
        //
        //    tempPacket=packet;
        
        network.write(s, payload, (int) strlen(payload) );
    }
    
    //if an event just ended, prepare log packet
    if ( eventEnd ) {
        eventEnd=false;
        //    sendPacket++;
        
        Serial.println("CRAFTING END PACKET");
        //insert header+payload code here
        //    char *bitmap = FLIP_construct_bitmap();
        //    char *header = FLIP_construct_header();
        
        //    logStr.toCharArray(logstr, 100);
        //    memset(packet, '\0', strlen(packet));
        //    char *packet = FLIP_construct_packet(bitmap,header,logstr);
        //    packet_len = get_packet_length();
        //
        //    tempPacket=packet;
        
        char *payload = "EVENT-ENDED";
        
        network.write(s, payload, (int) strlen(payload) );
    }
}
