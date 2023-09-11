#include <Arduino.h>
#include <BleKeyboard.h>
#include "pin_def.h"
#include <ClickEncoder.h>


// paramaters for buttons (not sure yet if really needed...)
unsigned long shortPressAfterMiliseconds = 50;   //how long short press shoud be. Do not set too low to avoid bouncing (false press events).
unsigned long longPressAfterMiliseconds = 1000;  //how long čong press shoud be.


// Encoder Test

#define ENCODER_COUNT 5
#define ENCODER_STEPS_PER_NOTCH    4   // Change this depending on which encoder is used

ClickEncoder encoder0(ENC1_A,ENC1_B,ENC1_BTN,ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder1(ENC2_A,ENC2_B,ENC2_BTN,ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder2(ENC3_A,ENC3_B,ENC3_BTN,ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder3(ENC4_A,ENC4_B,ENC4_BTN,ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder4(ENC5_A,ENC5_B,ENC5_BTN,ENCODER_STEPS_PER_NOTCH);

ClickEncoder encoders[ENCODER_COUNT] = {encoder0, encoder1, encoder2, encoder3, encoder4};

BleKeyboard bleKeyboard;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting work!");

  encoder0.setButtonHeldEnabled(true);
  encoder0.setDoubleClickEnabled(true);

  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending 'Hello world'...");
    bleKeyboard.print("Hello world");

    // delay(1000);

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);

    // delay(1000);

    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

    // delay(1000);

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

  // Serial.println("Waiting 5 seconds...");
  // delay(1000);


  // Encoder test
   for (int i = 0; i < ENCODER_COUNT; i++) {
    encoders[i].service();
    int16_t value = encoders[i].getValue();
    static int16_t lastValue[ENCODER_COUNT] = {0};
    if (value != lastValue[i]) {
      lastValue[i] = value;
      Serial.print("Encoder ");
      Serial.print(i + 1);
      Serial.print(" Value: ");
      Serial.println(value);
    }

    ClickEncoder::Button button = encoders[i].getButton();
    if (button != ClickEncoder::Open) {
      Serial.print("Button ");
      Serial.print(i + 1);
      Serial.print(": ");
      switch (button) {
        case ClickEncoder::Pressed:
          Serial.println("Pressed");
          break;
        case ClickEncoder::Held:
          Serial.println("Held");
          break;
        case ClickEncoder::Released:
          Serial.println("Released");
          break;
        case ClickEncoder::Clicked:
          Serial.println("Clicked");
          break;
        case ClickEncoder::DoubleClicked:
          Serial.println("DoubleClicked");
          break;
      }
    }
  }

}

