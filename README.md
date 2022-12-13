# ESP32-Pocsag-Pager
 Proof-of-concept Pager using a TTGO dev board. Will be merged with the "Waffle" project someday.
 
# Libraries
 * [RadioLib](https://github.com/jgromes/RadioLib) (temp ugly hack, but you must edit /modules/SX127X/SX127X.cpp, function readBit: updateDirectBuffer((uint8_t)!digitalRead(pin));)
 * Adafruit GFX (I plan to move tu U8G2 at some point)
 * Adafruiy SSD1306
 
 
# Hardware
Uses an ESP32 LORA32 TTGO. You must bridge DIO1 to pin 35 and DIO2 to pin 34 (broken out as LORA1 and LORA2 on the headers). 
I added a buzzer on pin 14, and plan to add buttons in the near future

# Next Steps
* ~~Add multi-address reception~~ Done.
* Configurable ringtones
* Implement a clean user interface to browse config, messages, etc. Currently experimenting with ArduinoMenu
* Create a wifi configuration portal
* Implement RTC
* Settings storage in SPIFFS