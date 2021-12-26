# ESP morse decoder

Morse decoder using an ESP8266 and an LDR via the ADC.

:warning: :warning: :warning: **This is a WIP!** :warning: :warning: :warning:


## Requirements

This project is intended to be used with [Platformio](https://platformio.org/) which will pull the required ESP8266 Arduino framework.


## Usage

  1. Wire up the LDR and a 100 K resistor in series to the 5 V supply as a voltage divider, hooking up the center contact to the ADC input

  ```
    ┌─────────│+5V│
    │
  LDR 
    │
    ├─────────│ADC│
    │
  100K
    │
    └─────────│GND│
  ```

  2. Enable `#define DEBUG` and upload the code to your ESP
  
  3. Connect via the serial terminal at 115200 baud and  tweak the signalThresh value until the output correctly shows 0 or 1 depending on your input

  3. While having `#define DEBUG` still enabled, tweak shortLongThresh to be slightly longer than a short pulse, 
     and nextLetterThresh to be slightly longer than the pause between two pulses. Also tweak letterWord thresh to be longer than the pause between two letters

  4. Comment out `#define DEBUG` again, upload to your ESP and restart the sequence. The sentence should print while being decoded.
