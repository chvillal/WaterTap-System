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


/* DEFINES */
#define RFM95_CS  8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 902.0
#define RF95_PWR  5

#define FLOWMETER A1
#define LEDPIN  13

/* GLOBAL VARIABLES */
enum states {
  INIT,
  SENSE,
  SLEEP
} topStateMachine;

volatile uint8_t pinState;
volatile uint8_t lastPinState;
unsigned long current_t;
unsigned long flowStart;
unsigned long eventDuration;
unsigned long pinChange;
unsigned long lastPinChange;
unsigned long pinDelta;
unsigned long nextPinCheck;
unsigned long nextSerialUpdate;
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t msg[RH_RF95_MAX_MESSAGE_LEN] = "SENSOR PACKET" ; //PLACEHOLDER ***
uint8_t buf_len;
uint8_t msg_len = 14;
uint8_t sendPacket;
float volume;
float flowRate;
float hertz;
bool flowing;
bool eventStart;
bool eventEnd;
char logstr[100];
int  packet_len;
String logStr;
char *tempPacket;
int s;

/* Single instance of the radio driver */
RH_RF95 rf95(RFM95_CS, RFM95_INT);
FlipKernel network;

bool toSend(uint8_t* data, uint8_t len){
  uint8_t data_len = len;
  return rf95.send(data, &data_len);
}

bool toRead(uint8_t* buf, uint8_t buf_len){
  return rf95.recv(buf, buf_len);
}

void setup() {
  
  //while (!Serial);
  Serial.begin(9600);
  lora_init();
  
  network.init(toSend, toRead);
  s = network.socket();
  
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_VERSION, 1);
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_DEST_1, 224);
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_TTL, 4);
  network.setsocketopt(s, SOCK_TYPE_FLIP, FLIP_SOURCE_1, 64);
  
  //flip_init();
  
  sensor_init();
  manager_init();
  
  topStateMachine = SENSE;
}

void loop() {
  switch (topStateMachine) {
    case INIT:
      break;
      
    case SENSE:
      current_t = millis();
      sensor_task();
      eventManager_task();
      loraRadio_task();
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

void sensor_init(void){
  //init flowmeter sensor pin
  pinMode(FLOWMETER, INPUT);
  digitalWrite(FLOWMETER, HIGH);

  //init sensor led pin
  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN, LOW);

  //init sensor variables
  nextPinCheck = millis();
  lastPinState = false;
  flowing = false;
  
}

void sensor_task(void){
  
  // Simulate 1ms interrupt, nextpincheck is in millis
  if ( current_t < nextPinCheck) {
    return;
  }
  
  //update next sensor check timeout value
  nextPinCheck++;

  //read sensor pin
  pinState = digitalRead(FLOWMETER);

  //if low to high transition
  if (pinState != lastPinState && pinState == HIGH){

    if (flowing == false){
      flowStart = current_t;
      Serial.println("FLOW EVENT STARTED");
      lastPinChange = current_t;
      flowing = true;
      lastPinState = pinState;
      digitalWrite(LEDPIN, HIGH);
      eventStart=true;
      return;
    }

    pinChange = current_t;
    pinDelta = pinChange - lastPinChange;

    if (pinDelta < 1000){
      hertz = 1000.0000 /pinDelta;
      flowRate = hertz/(60*7.5); // L/s
      volume += flowRate*(pinDelta/1000.0000);
    }

    lastPinChange = pinChange;
   
  } else if (flowing == true && pinState == lastPinState && 
                                (current_t - lastPinChange) > 3000){
    flowing = false;
    eventEnd=true;
    
    eventDuration = current_t - flowStart - 3000;
    String volumestr = String("L ");
    String volumeval = String(volume, 2);
    String durationstr = String("Sec");
    String durationval = String( eventDuration/1000 );

    logStr = volumeval + volumestr + durationval + durationstr;    
        
    volume = 0;
    Serial.println("FLOW EVENT FINISHED");
    Serial.println(logStr);
    digitalWrite(LEDPIN, LOW);
  }

  lastPinState = pinState;
}

void manager_init(void){
  eventStart=false;
  eventEnd=false;
  nextSerialUpdate = millis();

  buf_len = sizeof(buf);
  sendPacket = 0; 
}

void eventManager_task(void){
  
  // if flowing state active, print metrics every 1s
  if ( current_t >= nextSerialUpdate ){
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

void loraRadio_task(void){
  
//  //if packets queued for transmission, send 1
//  if ( sendPacket > 0){
//    rf95.send((uint8_t *) tempPacket, packet_len);
//    sendPacket--;
//  
//    Serial.println("SENDING PACKET!");
//    rf95.waitPacketSent();
  network.kernel();

  //else, check if we received anything  
  if (rf95.recv(buf, &buf_len)) {
    Serial.print("RECEIVED SOMETHING: ");
    Serial.println((char*)buf);
  }
}



