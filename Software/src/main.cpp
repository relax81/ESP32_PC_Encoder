#include <Arduino.h>
#include <BleKeyboard.h>
#include "pin_def.h"
#include <ClickEncoder.h>


// paramaters for buttons (not sure yet if really needed...)
unsigned long shortPressAfterMiliseconds = 50;   //how long short press shoud be. Do not set too low to avoid bouncing (false press events).
unsigned long longPressAfterMiliseconds = 1000;  //how long čong press shoud be.


// Encoder Test
#define ENCODER_STEPS_PER_NOTCH    4   // Change this depending on which encoder is used

ClickEncoder encoder = ClickEncoder(ENC1_A,ENC1_B,ENC1_BTN,ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder2 = ClickEncoder(ENC2_A,ENC2_B,ENC2_BTN,ENCODER_STEPS_PER_NOTCH);

BleKeyboard bleKeyboard;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Encoder test
  encoder.setButtonHeldEnabled(true);
  encoder.setDoubleClickEnabled(true);
  encoder2.setButtonHeldEnabled(true);
  encoder2.setDoubleClickEnabled(true);

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
    //Call Service in loop becasue using timer interrupts may affect ESP8266 WIFI
  //however call no more than 1 time per millisecond to reduce encoder bounce
  static uint32_t lastService = 0;
  if (micros() - lastService >= 1000) {
    lastService = micros();                
    encoder.service();  
    encoder2.service();
  }

 
static int16_t last, value;
    value += encoder.getValue();
  
  if (value != last) {
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);

  }

  static int16_t last2, value2;
    value2 += encoder2.getValue();
  
  if (value2 != last2) {
    last2 = value2;
    Serial.print("Encoder 2 Value: ");
    Serial.println(value2);

  }
  
  ClickEncoder::Button b = encoder.getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      VERBOSECASE(ClickEncoder::DoubleClicked)
    }
  }    

  ClickEncoder::Button b2 = encoder2.getButton();
  if (b2 != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b2) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      VERBOSECASE(ClickEncoder::DoubleClicked)
    }
  }  
}

