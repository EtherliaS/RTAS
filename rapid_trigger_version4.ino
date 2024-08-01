#include <Keyboard.h>
/*
 * Script for using rapid trigger on 2 button keypad.
 * Can be expanded to any number of buttons, depends on your arduino
*/

// - CONSTS -

// pins 
const uint8_t pin1 = A1;
const uint8_t pin2 = A2;
// keyboard code
const char key1code = 'x';
const char key2code = 'z';
//max value of pins (used for more convenient manual calibration, as well as at startup)
unsigned const int pin1max = 480; // min = 425 (~55)
unsigned const int pin2max = 490; // min = 450 (~40)
// activate points
unsigned const int k1activate = 465;
unsigned const int k2activate = 480;
// release points
unsigned const int k1release = 470;
unsigned const int k2release = 485;
//delay between keypresses in ms, used to avoid potencial double taps
unsigned const int Delay = 25;

// - VARS -

// phase checker
// the idea is that first the button signals that it is moving down
// and if when you check again it is still moving down - it is pressed
bool key2phase = false;
bool key1phase = false;

// delay checker avoidin doubletaps
unsigned long key2passedtime = 0;
unsigned long key1passedtime = 0;
unsigned long currentTime = 0;

// current key state
bool key1Pressed = false;
bool key2Pressed = false;

//startup values
int k1_values[3] {pin1max+10, pin1max+10, pin1max+10};
int k2_values[3] {pin2max+10, pin2max+10, pin2max+10};
int key2State = pin2max+10;
int key1State = pin1max+10;
int stage = 0;


// vector checker
// the idea is that if the button moves down consistently, then it is most likely pressed
int key2previousState2 = 0;
int key1previousState2 = 0;
int key2previousState = 0;
int key1previousState = 0;


void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  k1_values[stage] = analogRead(pin1);
  k2_values[stage] = analogRead(pin2);
  if (stage == 2){
    stage = 0;
  }
  key1State = avgk1();
  key2State = avgk2();
  checkPins();
  stage++;
  currentTime = micros();
}


void checkPins(){
  
  key2previousState2 = key2previousState;
  key2previousState = key2State;

  key1previousState2 = key1previousState;
  key1previousState = key1State;
  
  //k2
  if(key2State < k2activate){
    if (!key2Pressed && (key2State < key2previousState+1) && (key2previousState < key2previousState2+1) && (millis() - key2passedtime > Delay))
    {
      if(key2phase) PressKey2();
      key2phase = true;
    }
    else if (key2State > key2previousState && key2previousState > key2previousState2){
      if(!key2phase) ReleaseKey2();
      key2phase = false;
    }
    else if (key2State > k2release){
      key2phase = false;
      ReleaseKey2();
    }
  }
  else {
    key2phase = false;
    ReleaseKey2();
  }

  //k1
  if(key1State < k1activate){
    if (!key1Pressed && (key1State < key1previousState+1) && (key1previousState < key1previousState2+1) && (millis() - key1passedtime > Delay))
    {
      if(key1phase) PressKey1();
      key1phase = true;
    }
    else if (key1State > key1previousState && key1previousState > key1previousState2){
      if(!key1phase) ReleaseKey1();
      key1phase = false;
    }
    else if (key1State > k1release){
      key1phase = false;
      ReleaseKey1();
    }
  }
  else {
    key1phase = false;
    ReleaseKey1();
  }
}

int avgk1(){
  int sum = k1_values[0] + k1_values[1] + k1_values[2];
  return sum / 3;
}
int avgk2(){
  int sum = k2_values[0] + k2_values[1] + k2_values[2];
  return sum / 3;
}

void PressKey1(){
  if (!key1Pressed){
    Keyboard.press(key1code);
    key1Pressed = true;
  }
}

void ReleaseKey1(){
  if (key1Pressed){
    key1passedtime = millis();
    Keyboard.release(key1code);
    key1Pressed = false;
  }
}

void PressKey2(){
  if (!key2Pressed){
    Keyboard.press(key2code);
    key2Pressed = true;
  }
}

void ReleaseKey2(){
  if (key2Pressed){
    Keyboard.release(key2code);
    key2Pressed = false;
  }
}