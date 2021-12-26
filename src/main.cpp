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

//#define DEBUG

#include <Arduino.h>

// Constants
const int           signalThresh     =  400;  // Threshold between 0 and 1 on ADC
const unsigned long shortLongThresh  =  300;  // Threshold between short and long pulse in ms
const unsigned long nextLetterThresh =  500;  // Threshold to detect a new letter in ms
const unsigned long letterWordThresh = 1500;  // Threshold to detect a new word in ms
//#define INVERT                              // Define if you need to invert HIGH and LOW of your input signal


// Variables
int           brightness        =   0;  // Measured brightness
bool          signal;                   // Measured signal HIGH or LOW
bool          signalLast;               // Last cycle signal for debouncing
bool          signalDebounced;          // Current accepted debounced signal
bool          signalDebouncedLast;      // Last accepted debounced signal for edge detection
bool          sequenceEnded;            // Remember that sequence ended
unsigned long lastDebounceTime  =   0;  // The last time the signal was accepted
unsigned long debounceDelay     =  50;  // The debounce time; increase if the output flickers
unsigned long signalStartTime;          // To detect the length of a signal
unsigned long signalElapsedTime;        // Calculate the last signals time

String        currentLetter;            // Buffer for current letter
String        currentDecodedLetter;     // The last currentLetter buffer translated into a letter
String        sentence;                 // Buffer for full sentence


// Function prototypes
char decodeLetter(String signal);


/***********************************    Setup    ******************************************/
void setup() {

  // Start serial monitor
  Serial.begin(115200);
  
  // Wait for serial to connect
  while (!Serial) {
    ;
  }
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
  

  // Output debug information
  #ifdef DEBUG
    Serial.print("Signal: ");
    Serial.print(signalDebounced);
    Serial.print(" | Brightness: ");
    Serial.println(brightness);
  #endif


  //--------------------
  // Decode signal
  //--------------------

  // If signal state changed:
  if(signalDebounced != signalDebouncedLast){

    sequenceEnded = false;

    signalElapsedTime = millis() - signalStartTime; // Calculate the time of the last signal
    signalStartTime   = millis();                   // Remember start time of new signal

    // Output debug information
    #ifdef DEBUG
      Serial.print("LastSignal: ");
      Serial.print(signalDebouncedLast);
      Serial.print("Signal duration: ");
      Serial.println(signalElapsedTime);
    #endif

    // Last signal was HIGH - detect SHORT / LONG
    if(signalDebouncedLast == HIGH){

      // Detect short or long signal
      if(signalElapsedTime > shortLongThresh){
        currentLetter.concat("-");
        Serial.print("-");
      } else {
        currentLetter.concat(".");
        Serial.print(".");
      }
      #ifdef DEBUG
        Serial.println(); // line break after "-" or "."
      #endif


    // Last signal was LOW - detect LETTER / WORD
    } else {
      
       // Check if letter is finished
      if(signalElapsedTime > nextLetterThresh){
        // Add latest char
        sentence.concat(decodeLetter(currentLetter));
        currentLetter = "";

        // Print out what we got so far
        Serial.println();
        Serial.println(sentence);
        
        // Check if word is finished
        if(signalElapsedTime > letterWordThresh){
          // Add space after finished word
          sentence.concat(" ");
        }
      }
    }

   // Check if sentence is finished (signal does not change for a long time) then dump everything
  } else if ((millis() - signalStartTime) > letterWordThresh * 2){
    if (!sequenceEnded){
      sentence.concat(decodeLetter(currentLetter));

      currentLetter = "";

      // Print out what we found
      Serial.println();
      Serial.println("The sequence is:");
      Serial.println(sentence);
      sentence = "";

      sequenceEnded = true;
    }
  }

}


/***********************************    decodeLetter    ******************************************/
char decodeLetter(String signal) {
  // Decode a single letter signal pattern into actual character

  char returnLetter = '?';  // fallback initialization

  if        (signal == ".-")    {returnLetter = 'A';
  } else if (signal == "-...")  {returnLetter = 'B';
  } else if (signal == "-.-.")  {returnLetter = 'C';
  } else if (signal == "-..")   {returnLetter = 'D';
  } else if (signal == ".")     {returnLetter = 'E';
  } else if (signal == "..-.")  {returnLetter = 'F';
  } else if (signal == "--.")   {returnLetter = 'G';
  } else if (signal == "....")  {returnLetter = 'H';
  } else if (signal == "..")    {returnLetter = 'I';
  } else if (signal == ".---")  {returnLetter = 'J';
  } else if (signal == "-.-")   {returnLetter = 'K';
  } else if (signal == ".-..")  {returnLetter = 'L';
  } else if (signal == "--")    {returnLetter = 'M';
  } else if (signal == "-.")    {returnLetter = 'N';
  } else if (signal == "---")   {returnLetter = 'O';
  } else if (signal == ".--.")  {returnLetter = 'P';
  } else if (signal == "--.-")  {returnLetter = 'Q';
  } else if (signal == ".-.")   {returnLetter = 'R';
  } else if (signal == "...")   {returnLetter = 'S';
  } else if (signal == "-")     {returnLetter = 'T';
  } else if (signal == "..-")   {returnLetter = 'U';
  } else if (signal == "...-")  {returnLetter = 'V';
  } else if (signal == ".--")   {returnLetter = 'W';
  } else if (signal == "-..-")  {returnLetter = 'X';
  } else if (signal == "-.--")  {returnLetter = 'Y';
  } else if (signal == "--..")  {returnLetter = 'Z';
  } else if (signal == ".----") {returnLetter = '1';
  } else if (signal == "..---") {returnLetter = '2';
  } else if (signal == "...--") {returnLetter = '3';
  } else if (signal == "....-") {returnLetter = '4';
  } else if (signal == ".....") {returnLetter = '5';
  } else if (signal == "-....") {returnLetter = '6';
  } else if (signal == "--...") {returnLetter = '7';
  } else if (signal == "---..") {returnLetter = '8';
  } else if (signal == "----.") {returnLetter = '9';
  } else if (signal == "-----") {returnLetter = '0';
  }

  // Output debug information
  #ifdef DEBUG
    Serial.print("Found letter: ");
    Serial.print(signal);
    Serial.print(" | Decoded letter: ");
    Serial.println(returnLetter);
  #endif

  return returnLetter;

}