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
const int           signalThresh     = 400;  // Threshold between 0 and 1 on ADC
const unsigned long letterWordThresh = 800;  // Threshold to detect a new word in ms
const unsigned long shortLongThresh  = 500;  // Threshold between short and long pulse in ms
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
String        currentDecodedLetter;     // The last currentLetter buffer translated into a letter
String        currentWord;              // Buffer for current word
String        sentence;                 // Buffer for full sentence


// Function prototypes
char decodeLetter(String);


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
    signal = brightness > signalThresh;
  #else
    signal = brightness < signalThresh;
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
  

  // Decode
  if(signalDebounced != signalDebouncedLast){

    signalElapsedTime = millis() - signalStartTime; // Calculate the time of the last signal
    signalStartTime   = millis();                   // Remember start time of new signal

    // Last signal HIGH branch - LONG / SHORT
    if(signalDebouncedLast == HIGH){

      // Detect short or long signal
      if(signalElapsedTime > shortLongThresh){
        currentLetter.concat("-");
      } else {
        currentLetter.concat(".");
      }

    // Last signal LOW branch - detect LETTER / WORD
    } else {
      
      // Add latest char to word
      currentWord.concat(decodeLetter(currentLetter));

      // Check if word is finished
      if(signalElapsedTime > letterWordThresh){
        // Add latest word to sentence
        sentence.concat(currentWord);
        sentence.concat(" ");
      }

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


/***********************************    decodeLetter    ******************************************/
char decodeLetter(String) {
  // Decode a single letter signal pattern into actual character

  char returnLetter;

  // Output debug information
  #ifdef DEBUG
    Serial.print("Found letter: ");
    Serial.println(returnLetter);
  #endif

  return returnLetter;

}