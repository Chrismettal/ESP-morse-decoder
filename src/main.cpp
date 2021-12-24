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
bool          signalDebounced;          // Current accepted debounced signal
bool          signalDebouncedLast;      // Last accepted debounced signal for edge detection
unsigned long lastDebounceTime  =   0;  // The last time the signal was accepted
unsigned long debounceDelay     =  50;  // The debounce time; increase if the output flickers
unsigned long signalStartTime;          // To detect the length of a signal
unsigned long signalElapsedTime;        // Calculate the last signals time

String        currentLetter;            // Buffer for current letter
String        currentWord;              // Buffer for current word
String        sentence;                 // Buffer for full sentence

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
    signalDebouncedLast = signalDebounced;  // Remember last debounced signal for edge detection
    signalDebounced     = signal;           // Accept current signal as debounced
  }
  signalLast = signal;
  


    } else {}

    }late the time of the last signal
    signalStartTime   = millis();                   // Remember start time of new signal

    // Last signal HIGH branch - LONG / SHORT
    if(signalDebouncedLast == HIGH){
      
      if(signalElapsedTime > SHORT_LONG){
        currentWord.concat("-")
      } else {

      }

    // Last signal LOW branch - detect LETTER / WORD
    } else {
      
      ;

    }

  }

  // Output debug information
  #ifdef DEBUG
    Serial.print("Signal: ");
    Serial.print(signalDebounced);
    Serial.print(" | Brightness: ");
    Serial.println(brightness);
  #endif

}
