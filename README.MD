# ESP morse decoder

Morse decoder using an ESP8266 and an LDR via the ADC.

## Requirements

This project is intended to be used with [Platformio](https://platformio.org/) which will pull the required ESP8266 Arduino framework.


## Wiring

Wire up the LDR and a 100 K resistor in series to the 5 V supply as a voltage divider, hooking up the center contact to the ADC input

```
  ┌─────────│+5V│
  │
  │
 LDR 
  │
  │
  ├─────────│ADC│
  │
  │
 100K
  │
  │
  └─────────│GND│
```
