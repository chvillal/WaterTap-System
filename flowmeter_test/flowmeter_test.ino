//flow meter test.

#define FLOWMETER 2

volatile uint8_t lastflowmeterpinstate;
unsigned long current_t;
unsigned long nextPinCheck;
unsigned long nextSerialUpdate;
float volume;
float flowrate;
bool flowing;

void setup() {
  
  while(!Serial);
  Serial.begin(9600);

  Serial.print("Feather M0 Board - flowmeter test");
  
  pinMode(FLOWMETER, INPUT);
  digitalWrite(FLOWMETER, HIGH);
  lastflowmeterpinstate = digitalRead(FLOWMETER);
  nextPinCheck = millis();
  nextSerialUpdate = millis();

}

void loop() {
  current_t = millis();
  if ( current_t >= nextPinCheck ){
    nextPinCheck++;
    event_checker();
  }


  if ( current_t >= nextSerialUpdate ){
    nextSerialUpdate += 1000;

    if (flowing == true){
      Serial.println("FLOW UPDATE");
      Serial.print("Flow: ");
      Serial.println(flowrate);
      Serial.print("Volume: ");
      Serial.println(volume);
    }
  }  
}

void event_checker(void){

}


