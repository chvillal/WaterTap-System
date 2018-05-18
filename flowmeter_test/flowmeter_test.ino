//flow meter test.

#define FLOWMETER A2

volatile uint8_t lastflowmeterpinstate;
unsigned long current_t;
unsigned long flowStart;
unsigned long pinChange;
unsigned long lastPinChange;
unsigned long pinDelta;
unsigned long nextPinCheck;
unsigned long nextSerialUpdate;
float volume;
float flowRate;
float hertz;
bool flowing = false;
int pinState;
int lastPinState;

void setup() {
  
  while(!Serial);
  Serial.begin(9600);

  Serial.print("Feather M0 Board - flowmeter test");
  
  pinMode(FLOWMETER, INPUT);
  digitalWrite(FLOWMETER, HIGH);
  lastflowmeterpinstate = digitalRead(FLOWMETER);
  nextPinCheck = millis();
  nextSerialUpdate = millis();
  lastPinState = false;

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
      Serial.println(flowRate);
      Serial.print("Volume: ");
      Serial.println(volume);
    }
  }  
}

void event_checker(void){

  pinState = digitalRead(FLOWMETER);
  if (pinState != lastPinState && pinState == HIGH){

    if (flowing == false){
      flowStart = current_t;
      Serial.println("FLOW EVENT STARTED");
      lastPinChange = current_t;
      flowing = true;
      volume = 0;
      lastPinState = pinState;
      return;
    }

    pinChange = current_t;
    pinDelta = pinChange - lastPinChange;

    if (pinDelta < 1000){
      hertz = 1000/pinDelta;
      flowRate = hertz/(60*7.5); // L/s
      volume += flowRate*(pinDelta/1000);
    }
  }
  
  if (flowing == true && pinState == lastPinState && (current_t - lastPinChange) > 3000){
    flowing = false;
    Serial.println("FLOW EVENT FINISHED");
  }

  lastPinState = pinState;
}


