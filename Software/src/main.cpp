#include <Arduino.h>
#include <BleKeyboard.h>
#include "pin_def.h"
#include <AiEsp32RotaryEncoder.h>

// rotary Encoder
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 2

// paramaters for buttons
unsigned long shortPressAfterMiliseconds = 50;   //how long short press shoud be. Do not set too low to avoid bouncing (false press events).
unsigned long longPressAfterMiliseconds = 1000;  //how long čong press shoud be.


AiEsp32RotaryEncoder Encoder_1 = AiEsp32RotaryEncoder(ENC1_A, ENC1_B, ENC1_BTN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder Encoder_2 = AiEsp32RotaryEncoder(ENC2_A, ENC2_B, ENC2_BTN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder Encoder_3 = AiEsp32RotaryEncoder(ENC3_A, ENC3_B, ENC3_BTN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder Encoder_4 = AiEsp32RotaryEncoder(ENC4_A, ENC4_B, ENC4_BTN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder Encoder_5 = AiEsp32RotaryEncoder(ENC5_A, ENC5_B, ENC5_BTN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
BleKeyboard bleKeyboard;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending 'Hello world'...");
    bleKeyboard.print("Hello world");

    delay(1000);

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);

    delay(1000);

    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

    delay(1000);

   //
   // Below is an example of pressing multiple keyboard modifiers 
   // which by default is commented out.
    /*
    Serial.println("Sending Ctrl+Alt+Delete...");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_ALT);
    bleKeyboard.press(KEY_DELETE);
    delay(100);
    bleKeyboard.releaseAll();
    */
  }

  Serial.println("Waiting 5 seconds...");
  delay(5000);
}

