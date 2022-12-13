/*
ESP32 Pager Proof Of Concept
This code implements a basic pager, initally designed for DAPNET use, but can be modified to suit any need.

Additional files:
-config.h contains the user configuration (frequency, offset, RIC, ringtones, etc)
-periph.h contains pin assignment

Frequency offset must be configured for reliable decoding. At present time, there isn't a "cal" mode available, but it is to be implemented.


*/
#include "periph.h"
#include "config.h"
#include <RadioLib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




SX1278 radio = new Module(LORA_SS, LORA_DIO0, LORA_RST, LORA_DIO1);
// create Pager client instance using the FSK module
PagerClient pager(&radio);


#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);




void setup() {
  Serial.begin(115200);
  displayInit();
  ringBuzzer(STARTUPTONE);
  pocsagInit();
  pocsagStartRx();
}

void loop() {
  // the number of batches to wait for
  // 2 batches will usually be enough to fit short and medium messages
  if (pager.available() >= 2) {
    Serial.print(F("[Pager] Received pager data, decoding ... "));
    // you can read the data as an Arduino String
    String str;
    uint32_t addr = 0;
    int state = pager.readData(str, 0, &addr);
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));

      // print the received data
      Serial.print(F("[Pager] Address:\t"));
      Serial.print(String(addr));
      Serial.print(F("[Pager] Data:\t"));
      Serial.println(str);

      for (int i = 0; i < RICNUMBER; i++) {
        if (addr == ric[i].ricvalue) {
          displayPage(ric[i].name, str);
          ringBuzzer(ric[i].ringtype);
        }
      }


    } else {
      // some error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
  }
}


void displayPage(String address, String text) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.print(String(address));
  display.setCursor(0, 30);
  display.print(text);
  display.display();
}

//Generates the tones based on predefined settings in config.h
void ringBuzzer(int ringToneChoice) {
  for (int i = 0; i < NOTENUMBER; i++) {
    tone(BUZZER, beepTones[ringToneChoice][i], 130);
  }
}


void displayInit() {

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true)
      ;  // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.print("PAGER RX Proof of Concept ");
  display.display();
}




void pocsagInit() {
  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }

  // initialize Pager client
  Serial.print(F("[Pager] Initializing ... "));
  state = pager.begin(frequency + offset, 1200);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}

void pocsagStartRx() {
  // start receiving POCSAG messages
  Serial.print(F("[Pager] Starting to listen ... "));
  // address of this "pager":     1234567
  int state = pager.startReceive(LORA_DIO2, 200, 0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}