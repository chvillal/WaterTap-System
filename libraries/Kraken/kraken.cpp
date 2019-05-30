//
//  kraken.cpp
//  eFLIP
//
//  Created by Christopher Villalpando on 5/25/19.
//  Copyright © 2019 Christopher Villalpando. All rights reserved.
//

#include "kraken.h"
//#include <stdio.h>
//#include <string>

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

//void WaterSensor::enableLED(uint8_t pin) 
//{
    ////init sensor led pin
    //if (ledEnabled == false){
        //ledPin = pin;
        //pinMode(ledPin, OUTPUT);
        //digitalWrite(ledPin, LOW);
        //ledEnabled = true;
    //}
//}

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
			volume = 0;
            flowStart = time_m;
            //Serial.println("FLOW EVENT STARTED");
            lastPinChange = time_m;
            flowing = true;
            lastPinState = pinState;
            eventStart=true;
            //if (ledEnabled){
                //digitalWrite(ledPin, HIGH);
            //}
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
               (time_m - lastPinChange) > EVENT_TIMEOUT){
        flowing = false;
        eventEnd=true;
        
        eventDuration = time_m - flowStart - EVENT_TIMEOUT;
		
		//print to serial screen
        String volumestr = String("L ");
        String volumeval = String(volume, 2);
        String durationstr = String("Sec");
        String durationval = String( eventDuration/1000 );
        logStr = volumeval + volumestr + durationval + durationstr;
        
        //volume = 0;
        Serial.print("FLOW EVENT FINISHED: ");
        Serial.println(logStr);
		
        //if (ledEnabled){
        //    digitalWrite(ledPin, LOW);
        //}
        
    }
    
    lastPinState = pinState;
}

void WaterSensor::reset_metrics(void)
{
	volume = 0;
	flowRate = 0;
}

void WaterSensor::reset_event(void)
{
	eventStart = false;
	eventEnd = false;
}


//void manager_init(void){
    //eventStart=false;
    //eventEnd=false;
    //nextSerialUpdate = millis();
    //
    //buf_len = sizeof(buf);
    //sendPacket = 0;
//}


//void eventManager_task(unsigned long time_m){
    //
    //// if flowing state active, print metrics every 1s
    //if ( time_m >= nextSerialUpdate ){
        //nextSerialUpdate += 1000;
        //if (flowing == true){
            //Serial.println("FLOW UPDATE");
            //Serial.print("Flow: ");
            //Serial.println(flowRate);
            //Serial.print("Volume: ");
            //Serial.println(volume);
            //
        //}
    //}
    //
    //// if an event just started, prepare notification packet
    //if ( eventStart ){
        //eventStart=false;
		//
        //Serial.println("CRAFTING START PACKET");
        //char *payload = "EVENT-START";
//
        //network.write(s, payload, (int) strlen(payload) );
    //}
    //
    ////if an event just ended, prepare log packet
    //if ( eventEnd ) {
        //eventEnd=false;
        //
        //Serial.println("CRAFTING END PACKET");
        //char *payload = "EVENT-ENDED";
        //
        //network.write(s, payload, (int) strlen(payload) );
    //}
//}
