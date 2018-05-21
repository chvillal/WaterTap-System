// flow meter test
// 
// Connect flowmeter sensor to power, gnd, and signal to  pin A2
//
// this sketch tests operation of the flowmeter sensor. prints metrics
// to serial COM monitor.
//
// authors:       christopher villalpando, andrea david
// emails:        chvillal@ucsc.edu, andavid@ucsc.edu
// last-modified: 5/21/2018

#define FLOWMETER A2
#define LEDPIN    13

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

  Serial.println("Feather M0 Board - flowmeter test");

  pinMode(LEDPIN,OUTPUT); //LED SETUP
  digitalWrite(LEDPIN, LOW);
  
  pinMode(FLOWMETER, INPUT);
  digitalWrite(FLOWMETER, HIGH);
  lastflowmeterpinstate = digitalRead(FLOWMETER);
  nextPinCheck = millis();
  nextSerialUpdate = millis();
  lastPinState = false;

}

void loop() {

  /* simulates 1 ms interrupt, run event checker */
  current_t = millis();
  if ( current_t >= nextPinCheck ){
    nextPinCheck++;
    event_checker();
  }

  /* if flowing state active, print metrics every 1s */
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
      lastPinState = pinState;
      digitalWrite(LEDPIN, HIGH);
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
   
  } else if (flowing == true && pinState == lastPinState && (current_t - lastPinChange) > 3000){
    flowing = false;
    volume = 0;
    Serial.println("FLOW EVENT FINISHED");
    digitalWrite(LEDPIN, LOW);
  }

  lastPinState = pinState;
}


