#include <Keyboard.h>
/*
 * Script for using rapid trigger on 2 button keypad.
 * Can be expanded to any number of buttons, depends on your arduino
*/
// - CONSTS -
// pins and keyboard codes
const uint8_t pins[] = {A1, A2};
const char keyCodes[] = {'x', 'z'};
// max values of pins (used for manual calibration, as well as at startup)
const unsigned int pinMax[] = {480, 490}; // min = 425 (~55), min = 450 (~40)
// activation and release points
const unsigned int activatePoints[] = {465, 480};
const unsigned int releasePoints[] = {470, 485};
// delay between keypresses in ms, used to avoid potential double taps
const unsigned int pressDelay = 25;
// smoothing factor for EMA
const float alpha = 0.2;

// - VARS -
bool keyPhases[] = {false, false};
unsigned long keyPassedTime[] = {0, 0};
bool keyPressed[] = {false, false};
float emaStates[] = {0, 0};  // array to store EMA values
int previousStates[2][2];
int stage = 0;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  for (int i = 0; i < 2; i++) {
    emaStates[i] = pinMax[i] + 10;
    previousStates[i][0] = previousStates[i][1] = 0;
  }
}

void loop() {
  for (int i = 0; i < 2; i++) {
    int currentValue = analogRead(pins[i]);
    emaStates[i] = alpha * currentValue + (1 - alpha) * emaStates[i];
    checkPin(i);
  }
}

void checkPin(int index) {
  previousStates[index][1] = previousStates[index][0];
  previousStates[index][0] = emaStates[index];

  if (emaStates[index] < activatePoints[index]) {
    if (!keyPressed[index] && isMovingDown(index) && (millis() - keyPassedTime[index] > pressDelay)) {
      if (keyPhases[index]) pressKey(index);
      keyPhases[index] = true;
    } else if (isMovingUp(index)) {
      if (!keyPhases[index]) releaseKey(index);
      keyPhases[index] = false;
    } else if (emaStates[index] > releasePoints[index]) {
      keyPhases[index] = false;
      releaseKey(index);
    }
  } else {
    keyPhases[index] = false;
    releaseKey(index);
  }
}

bool isMovingDown(int index) {
  return emaStates[index] < previousStates[index][0] + 1 && previousStates[index][0] < previousStates[index][1] + 1;
}

bool isMovingUp(int index) {
  return emaStates[index] > previousStates[index][0] && previousStates[index][0] > previousStates[index][1];
}

void pressKey(int index) {
  if (!keyPressed[index]) {
    Keyboard.press(keyCodes[index]);
    keyPressed[index] = true;
  }
}

void releaseKey(int index) {
  if (keyPressed[index]) {
    keyPassedTime[index] = millis();
    Keyboard.release(keyCodes[index]);
    keyPressed[index] = false;
  }
}
