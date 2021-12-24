/*
  ESP morse decoder

  1. Wire up the LDR and a 100 K resistor in series to the 5 V supply as a voltage divider, hooking up the center contact to the ADC input

    ┌─────────│+5V│
    │
  LDR 
    │
    ├─────────│ADC│
    │
  100K
    │
    └─────────│GND│

  2. Enable `#define DEBUG` below and tweak the THRESHOLD value until the output correctly shows LOW or HIGH depending on your input

*/

#define DEBUG

#include <Arduino.h>

// Constants
#define THRESHOLD     400;  // Threshold between 0 and 1 on ADC
#define SHORT_LONG    500;  // Threshold between short and long pulse in ms
#define TIME_WORDS    800;  // Threshold to detect a new word in ms
//#define INVERT            // Define if you need to invert HIGH and LOW

// Variables
int           brightness        =   0;  // Measured brightness
bool          signal;                   // Measured signal HIGH or LOW
bool          signalLast;               // Last cycle signal for debouncing
bool          signalDebounced;          // last accepted debounced signal
unsigned long lastDebounceTime  =   0;  // the last time the signal was accepted
unsigned long debounceDelay     =  50;  // the debounce time; increase if the output flickers


/***********************************    Setup    ******************************************/
void setup() {

  // Start serial monitor
  Serial.begin(115200);

}


/***********************************     Loop    ******************************************/
void loop() {

  // Read in brightness level
  brightness = analogRead(A0);


  // Detect HIGH or LOW signal
  #ifndef INVERT
    signal = brightness > THRESHOLD;
  #else
    signal = brightness < THRESHOLD;
  #endif


  // Debounce signal
  if (signal != signalLast) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    signalDebounced = signal;
  }
  signalLast = signal;


  // Output debug information
  #ifdef DEBUG
    Serial.print("Signal: ");
    Serial.print(signalDebounced);
    Serial.print(" | Brightness: ");
    Serial.println(brightness);
  #endif

}
