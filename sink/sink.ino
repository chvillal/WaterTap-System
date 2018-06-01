/*
 * name:   sink.ino
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

/* DEFINES */
#define RFM95_CS  8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 902.0
#define RF95_PWR  5

/* GLOBAL VARIABLES */
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t msg[RH_RF95_MAX_MESSAGE_LEN] = "ACK" ; //PLACEHOLDER ***
uint8_t buf_len = sizeof(buf);
uint8_t msg_len = 4;

/* Single instance of the radio driver */
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  while (!Serial);
  Serial.begin(9600);

  lora_init();
}

void loop() {
  receivePacket();  

}

void lora_init(void)
{
  //LoRa reset pin
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.println("WATERTAP SYSTEMS - SINK NODE");

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

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  // set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(RF95_PWR, false);
}

void receivePacket(void){

  if ( rf95.recv(buf, &buf_len) ){
    Serial.print("RECEIVED: ");
    Serial.println((char*)buf);
    Serial.print("RSSI: ");
    Serial.println(rf95.lastRssi(), DEC);

    
    rf95.send((uint8_t *)msg, msg_len);
    rf95.waitPacketSent();
    Serial.println("ACK SENT");
    Serial.println(" ");
  }
  
}


