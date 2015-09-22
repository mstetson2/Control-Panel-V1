// constants used here to set pin numbers:
const int dispatchButton = 2;
const int eStopButton = 3;
const int resetButton = 4;
const int gateSwitch = 5;
const int restraintSwitch = 6;
const int keySwitch = 7;
const int dispatchLed = 8;
const int eStopLed = 9;
const int resetLed = 10;
const int powerLed = 11;
const int troubleLed = 12;
// PIN 13

// variables will change:
int dispatchButtonState = 0;
int eStopButtonState = 0;
int resetButtonState = 0;
int gateSwitchState = 0;
int restraintSwitchState = 0;
int keySwitchState = 0;

boolean noLimitsStarted = false;

boolean gatesOpen = false;
boolean restraintsOpen = false;

boolean dispatchPressed = false;
boolean resetPressed = false;
boolean eStopPressed = false;
boolean eStopSent = false;
boolean gateSwitchTurned = false;
boolean restraintSwitchTurned = false;
boolean powerOn = false;

//Blinking led setup:
//stores last time LED was updated
long previousMillis = 0;
//intervals
long longInterval = 1000;
long shortInterval = 500;


void setup() {
  //initialize the serial port
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(dispatchLed, OUTPUT);
  pinMode(eStopLed, OUTPUT);
  pinMode(resetLed, OUTPUT);
  pinMode(powerLed, OUTPUT);
  pinMode(troubleLed, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(dispatchButton, INPUT);  
  pinMode(eStopButton, INPUT);
  pinMode(resetButton, INPUT);
  pinMode(gateSwitch, INPUT);
  pinMode(restraintSwitch, INPUT);
  pinMode(keySwitch, INPUT);

  //initialize the buttonPin as output
  digitalWrite(dispatchButton, HIGH);
  digitalWrite(eStopButton, HIGH);
  digitalWrite(resetButton, HIGH);
  digitalWrite(gateSwitch, HIGH);
  digitalWrite(restraintSwitch, HIGH);
  digitalWrite(keySwitch, HIGH);

}

void loop(){
  // read the state of the pushbutton value:
  dispatchButtonState = digitalRead(dispatchButton);
  eStopButtonState = digitalRead(eStopButton);
  resetButtonState = digitalRead(resetButton);
  gateSwitchState = digitalRead(gateSwitch);
  restraintSwitchState = digitalRead(restraintSwitch);
  keySwitchState = digitalRead(keySwitch);

  unsigned long currentMillis = millis(); 
  ////////////////////////////////////////////////////// 
  //             Lighting Effects                     // 
  ////////////////////////////////////////////////////// 
  //if e-stop is pressed:
  if(eStopPressed == true)  {
    digitalWrite(dispatchLed,LOW); 
    if(currentMillis - previousMillis > longInterval) { 
      // save the last time you blinked the LED  
      previousMillis = currentMillis;    

      // if the e-stop is in down position:
      if (eStopButtonState == LOW){ 
        digitalWrite(eStopLed, HIGH); 
      } 
      else {
        if(digitalRead(eStopLed) == LOW){
          digitalWrite(eStopLed, HIGH); 
        }
        else{
          digitalWrite(eStopLed, LOW);     
        }
      } 
    }
    if(digitalRead(troubleLed) == LOW)  {
      digitalWrite(troubleLed, HIGH);
      digitalWrite(powerLed, HIGH);
      if(eStopButtonState == HIGH){
        digitalWrite(resetLed, HIGH); 
      }
    }
    else {
      digitalWrite(troubleLed, LOW);
      digitalWrite(powerLed, LOW);
      if(eStopButtonState == HIGH){
        digitalWrite(resetLed, LOW); 
      }
    }
  }

  if(gatesOpen == false && restraintsOpen == false && eStopPressed == false) {
    if(currentMillis - previousMillis > longInterval) { 
      previousMillis = currentMillis;    

      if (digitalRead(dispatchLed) == LOW) 
        digitalWrite(dispatchLed, HIGH); 
      else 
        digitalWrite(dispatchLed, LOW); 
    }
  }


  //BUTTONS AND SWITCHES\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


  if(keySwitchState == LOW){
    if(eStopPressed == false){
      if(powerOn == false){
      digitalWrite(powerLed, HIGH);
      powerOn = true;
      }
      else if(noLimitsStarted == false && resetButtonState == LOW && dispatchButtonState == LOW) {
        Serial.println("#S|STARTNOLIM|[]#");
        noLimitsStarted = true;   
        for(int i=0;i<=3;i++){ 
          // Blink all lights 
          delay(shortInterval); 
          digitalWrite(powerLed, LOW); 
          delay(shortInterval); 
          digitalWrite(powerLed, HIGH);
        }
      }
    }
  }
  else{
    digitalWrite(powerLed, LOW);
    eStopPressed = true;
  }

  //dispatch button
  if (dispatchButtonState == LOW) {
    digitalWrite(dispatchLed, HIGH);
    if(dispatchPressed == false){
      //send dispatch key
      Serial.print("#S|SENDK|[{ENTER}]#");     
      dispatchPressed = true;
    }
  }
  else  {
    dispatchPressed = false;
  }

  //reset button
  if (resetButtonState == LOW) {
    if(eStopPressed == true){
      if(digitalRead(eStopButtonState) == HIGH){
        digitalWrite(resetLed, HIGH);
        digitalWrite(powerLed, HIGH);
        digitalWrite(troubleLed, LOW);
        if(resetPressed == false) {
          Serial.print("#S|SENDK|[{F12}]#");
          resetPressed = true;
          eStopPressed = false;
          eStopSent = false;
        }
      }
    }
    else if(resetButtonState == LOW && restraintSwitchState == LOW && gateSwitchState == LOW){
      Serial.print("#S|SENDK|[{F11}]#"); 
    }
  }
  else {
    resetPressed = false;
    digitalWrite(resetLed, LOW);
  }

  //E-Stop
  if(eStopButtonState == LOW){
    eStopPressed = true;
    if(eStopSent == false) {
      Serial.print("#S|SENDK|[{F12}]#");
      eStopSent = true;
    }
  }

  //Gate Toggle Switch
  if(gateSwitchState == LOW){
    if(gatesOpen == false){
      Serial.print("#S|SENDK|[{F10}]#");
      gatesOpen = true;
    }
  }
  else {
    if(gatesOpen == true){
      Serial.print("#S|SENDK|[{F10}]#");
      gatesOpen = false; 
    }
  }

  //RESTRAINT SWITCH
  if(restraintSwitchState == LOW){
    if(restraintsOpen == false){
      Serial.print("#S|SENDK|[{F9}]#");
      restraintsOpen = true;
    }
  }
  else {
    if(restraintsOpen == true){
      Serial.print("#S|SENDK|[{F9}]#");
      restraintsOpen = false; 
    }
  }

  delay(1);
}


