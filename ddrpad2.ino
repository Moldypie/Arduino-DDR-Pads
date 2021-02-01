/* Arduino DDR pad for Arduino Leonardo
 * 8 Buttons + 8 HID controlable LED
 * 
 * based on code written by KnucklesLee
 * http://knuckleslee.blogspot.com/2018/06/RhythmCodes.html
 * 
 * and code written by Mon
 * https://github.com/mon/Arduino-HID-Lighting
 * 
 * Arduino Joystick Library
 * https://github.com/MHeironimus/ArduinoJoystickLibrary/
 */
#include <Joystick.h>
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD, 8, 0,
 false, false, false, false, false, false, false, false, false, false, false);

boolean updateLights = false;
boolean hidMode = false;
byte SinglePins[] = {10,16,14,15,A0,A1,A2,A3};
byte ButtonPins[] = {2,3,4,5,6,7,8,9};
unsigned long idleTimerMax = 30000;
unsigned long idleTimer = 0;
unsigned long lightsCounter = 0;


byte ButtonCount = sizeof(ButtonPins) / sizeof(ButtonPins[0]);
byte SingleCount = sizeof(SinglePins) / sizeof(SinglePins[0]);

void setup() {
  Serial.begin(9600) ;
  Joystick.begin(false);
  
  // setup I/O for pins
  for(int i=0;i<ButtonCount;i++) {
    pinMode(ButtonPins[i],INPUT_PULLUP);
  }
  for(int i=0;i<SingleCount;i++) {
    pinMode(SinglePins[i],OUTPUT);
  }


  while(digitalRead(ButtonPins[0])==LOW) {
    if ( (millis() % 1000) < 500) {
      for(int i=0;i<SingleCount;i++) {
        digitalWrite(SinglePins[i],HIGH);
      }
    }
    else if ( (millis() % 1000) > 500) {
      for(int i=0;i<SingleCount;i++) {
        digitalWrite(SinglePins[i],LOW);
      }
    }
  }
  for(int i=0;i<SingleCount;i++) {
    digitalWrite(SinglePins[i],LOW);
  }

  //boot light
  for(int i=0;i<ButtonCount ;i++) {
    digitalWrite(SinglePins[i],HIGH);
  }
    delay(500);
  for(int i=0;i<ButtonCount ;i++) {
    digitalWrite(SinglePins[i],LOW);
  }
} //end setup

void loop() {
  if (lightsCounter > 500) {
    hidMode = false;
  }else{
    hidMode = true;
  }
  if(hidMode == false && updateLights == false){
    for(int i=0;i<ButtonCount;i++) {
      digitalWrite(SinglePins[i],!(digitalRead(ButtonPins[i])));
    }
    //Serial.println("non HID light off");
  }
  // read buttons
  for(int i=0;i<ButtonCount;i++) {
    if(digitalRead(ButtonPins[i])==LOW) {
      Joystick.setButton (i,1);
    } else {
      Joystick.setButton (i,0);
    }
  }
  //when buttons aren't touched, increase the idle timer, otherwise reset it to 0
  if((digitalRead(ButtonPins[0]) == 0 || digitalRead(ButtonPins[1]) == 0 || digitalRead(ButtonPins[2]) == 0 || digitalRead(ButtonPins[3]) == 0 || digitalRead(ButtonPins[4]) == 0 || digitalRead(ButtonPins[5]) == 0 || digitalRead(ButtonPins[6]) == 0 || digitalRead(ButtonPins[7]) == 0) || hidMode == true){
    idleTimer = 0;
  }else{
    //Serial.println(idleTimer);
    idleTimer++;
  }
  //play idle animation and break if a button is touched
  if(idleTimer >= idleTimerMax && hidMode == false){
    for(int i=0;i<10000;i++){
      if(digitalRead(ButtonPins[0])==LOW || digitalRead(ButtonPins[1])==LOW || digitalRead(ButtonPins[2])==LOW || digitalRead(ButtonPins[3])==LOW || digitalRead(ButtonPins[4])==LOW || digitalRead(ButtonPins[5])==LOW || digitalRead(ButtonPins[6])==LOW || digitalRead(ButtonPins[7])==LOW){
        break;
      }
      digitalWrite(SinglePins[0],HIGH);
      digitalWrite(SinglePins[1],HIGH);
      digitalWrite(SinglePins[6],HIGH);
      digitalWrite(SinglePins[7],HIGH);
      digitalWrite(SinglePins[2],LOW);
      digitalWrite(SinglePins[3],LOW);
      digitalWrite(SinglePins[4],LOW);
      digitalWrite(SinglePins[5],LOW);
    }
    for(int i=0;i<10000;i++){
      if(digitalRead(ButtonPins[0])==LOW || digitalRead(ButtonPins[1])==LOW || digitalRead(ButtonPins[2])==LOW || digitalRead(ButtonPins[3])==LOW || digitalRead(ButtonPins[4])==LOW || digitalRead(ButtonPins[5])==LOW || digitalRead(ButtonPins[6])==LOW || digitalRead(ButtonPins[7])==LOW){
        break;
      }
      digitalWrite(SinglePins[0],LOW);
      digitalWrite(SinglePins[1],LOW);
      digitalWrite(SinglePins[6],LOW);
      digitalWrite(SinglePins[7],LOW);
      digitalWrite(SinglePins[2],HIGH);
      digitalWrite(SinglePins[3],HIGH);
      digitalWrite(SinglePins[4],HIGH);
      digitalWrite(SinglePins[5],HIGH);
    }
  }

  if (updateLights){
    lightsCounter = 0;
    updateLights = false;
  }else{
    lightsCounter++;
  }
  
  //report
  Joystick.sendState();
}
