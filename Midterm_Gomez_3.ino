/*======================================================
    BIDIRECTIONAL IR SENSOR COUNTER WITH 7 SEGMENT DISPLAY
========================================================
Description:
  This program reads digital signals from two infrared
  obstacle sensors to count detected objects bidirectionally.
  One sensor increments the count up to 99, while the other
  sensor decrements the count down to 0. A push button resets
  the count to 3. The system outputs the real-time count
  onto a dual common-cathode 7-segment display.

Programmer: 
  Sergie D. Gomez

Date: 
  28 October 2026
------------------------------------------------------*/
```
const uint8_t IR_UP_PIN = 22;
const uint8_t IR_DOWN_PIN = 23;
const uint8_t RESET_PIN = 13;

const uint8_t TENS_PINS[] = {4, 16, 17, 5, 18, 19, 21};
const uint8_t ONES_PINS[] = {32, 33, 25, 26, 27, 14, 12};

const uint8_t NUM_SEG7 = sizeof(TENS_PINS) / sizeof(TENS_PINS[0]);

#include "decoder.h"

int Number_of_Object = 0;

int lastUpState = HIGH;
int lastDownState = HIGH;
int lastResetState = LOW;

void setup() {
  Serial.begin(9600);

  pinMode(IR_UP_PIN, INPUT);
  pinMode(IR_DOWN_PIN, INPUT);
  pinMode(RESET_PIN, INPUT_PULLDOWN);

  for (int i = 0; i < NUM_SEG7; i++) {
    pinMode(TENS_PINS[i], OUTPUT);
    pinMode(ONES_PINS[i], OUTPUT);
  }

  decoder(Number_of_Object);
}

void loop() {
  int upState = digitalRead(IR_UP_PIN);
  int downState = digitalRead(IR_DOWN_PIN);
  int resetState = digitalRead(RESET_PIN);

  if (resetState == HIGH) {
    Number_of_Object = 3;
    decoder(Number_of_Object);
  }
  else {
    if (upState == LOW && lastUpState == HIGH) {
      Number_of_Object++;

      if (Number_of_Object > 99) {
        Number_of_Object = 99;
      }

      decoder(Number_of_Object);

      Serial.printf("Count up: %d\n", Number_of_Object);

      delay(200);
    }

    if (downState == LOW && lastDownState == HIGH) {
      Number_of_Object--;

      if (Number_of_Object < 0) {
        Number_of_Object = 0;
      }

      decoder(Number_of_Object);

      Serial.printf("Count down: %d\n", Number_of_Object);

      delay(200);
    }
  }

  lastUpState = upState;
  lastDownState = downState;
  lastResetState = resetState;
}
