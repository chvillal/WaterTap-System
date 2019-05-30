/*
 * name:   sensor.ino
 * board:  Feather M0
 *
 * authors:  christopher villalpando, andrea david
 * emails:   chvillal@ucsc.edu, andavid@ucsc.edu
 * 
 * description:
 *
 */

/* LIBRARIES */
#include <SPI.h>
#include <RH_RF95.h>
#include <eflip.h>
#include <kraken.h>


/* DEFINES */
#define RFM95_CS  8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 902.0
#define RF95_PWR  5

#define FLOWMETER1 A1
#define FLOWMETER2 A2
#define LEDPIN  13

/* GLOBAL VARIABLES */
enum states {
  INIT,
  SENSE,
  SLEEP
} topStateMachine;

unsigned long nextSerialUpdate;
unsigned long current_t;
int s;

/* Single instance of the radio driver */
RH_RF95 rf95(RFM95_CS, RFM95_INT);
FlipKernel network;
WaterSensor sensor1;
WaterSensor sensor2;

//function wrapper for FLIP
bool toSend(uint8_t* data, uint8_t len){
  return rf95.send(data, len);
}

//function wrapper for FLIP
bool toRead(uint8_t* buf, uint8_t* len){
  return rf95.recv(buf, len);
}

void setup() {
  
  while (!Serial);
  Serial.begin(9600);

  //lora radio init
  lora_init();

  //network socket init
  network.init(toSend, toRead);
  s = network.socket();
  
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_VERSION, 1);
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_DEST_1, 224);
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_TTL, 4);
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_SOURCE_1, 64);

  //system modules init
  sensor1.init(FLOWMETER1);
  sensor2.init(FLOWMETER2);
  nextSerialUpdate = millis();
  
  topStateMachine = SENSE;
}

void loop() {
  switch (topStateMachine) {
    case INIT:
      break;
      
    case SENSE:
      current_t = millis();
      //sensor_task();
      sensor1.task(current_t);
      sensor2.task(current_t);
      eventManager_task();
      network.kernel();
      break;
      
    case SLEEP:
      break;      
    default:
      break;
  }
}

void lora_init(void)
{
  //LoRa reset pin
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.println("WATERTAP SYSTEMS - SENSOR NODE");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // wait for Lora radio to init
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  rf95.setTxPower(RF95_PWR, false);
}

void eventManager_task(void){
  
  // if flowing state active, print metrics every 1s
  if ( current_t >= nextSerialUpdate ){
    nextSerialUpdate += 1000;
    
    if ( sensor1.isEventFlowing() == true){
      Serial.print("SENSOR 1 FLOWUPDATE: ");
      Serial.print("Rate: ");
      Serial.print( sensor1.get_flowRate() );
      Serial.print(" Volume: ");
      Serial.println( sensor1.get_volume() );

    }
    if ( sensor2.isEventFlowing() == true){
      Serial.print("SENSOR 2 FLOWUPDATE: ");
      Serial.print("Rate: ");
      Serial.print( sensor2.get_flowRate() );
      Serial.print(" Volume: ");
      Serial.println( sensor2.get_volume() );

    }
  }

  // if an event just started, prepare notification packet
  if ( sensor1.isEventStart() ){
    sensor1.clrEventStart();

    Serial.println("SENSOR 1 CRAFTING START PACKET");
    char *payload = "EVENT-START-SENSOR-1";
    network.write(s, payload, (int) strlen(payload) );
  }
  if ( sensor2.isEventStart() ){
    sensor2.clrEventStart();

    Serial.println("SENSOR 2 CRAFTING START PACKET");
    char *payload = "EVENT-START-SENSOR-2";
    network.write(s, payload, (int) strlen(payload) );
  }

  //if an event just ended, prepare log packet
  if ( sensor1.isEventEnd() ) {
    sensor1.clrEventEnd();
    
    Serial.println("SENSOR 1 CRAFTING END PACKET");
    char *payload = "EVENT-ENDED-SENSOR-1";
    network.write(s, payload, (int) strlen(payload) );
  }  
  if ( sensor2.isEventEnd() ) {
    sensor2.clrEventEnd();
    
    Serial.println("SENSOR 2 CRAFTING END PACKET");
    char *payload = "EVENT-ENDED-SENSOR-2";
    network.write(s, payload, (int) strlen(payload) );
  }
}

