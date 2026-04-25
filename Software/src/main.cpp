// NOTE: For F13-F24 keys to work, edit BleKeyboard.cpp in the library:
// Change logical_maximum and usage_maximum from 0x65 to 0x73

#include <Arduino.h>
#include <BleKeyboard.h>
#include "pin_def.h"
#include <ClickEncoder.h>

// ================================================================
// Configuration
// ================================================================

// Number of encoder knobs connected
#define ENCODER_COUNT 5

// Steps per physical notch/click — depends on your encoder model.
// Most common values are 2 or 4. Turn an encoder and check Serial
// Monitor: if one notch = 2 steps shown, set this to 2.
#define ENCODER_STEPS_PER_NOTCH 4

// ================================================================
// Key Mapping
// ================================================================
// Edit these arrays to change which keys are sent.
// Index 0 = Encoder 1, Index 1 = Encoder 2, etc.

// Turning right  →  CTRL + this key
const uint8_t KEY_TURN_RIGHT[ENCODER_COUNT] = {KEY_F13, KEY_F15, KEY_F17, KEY_F19, KEY_F21};

// Turning left   →  CTRL + this key
const uint8_t KEY_TURN_LEFT[ENCODER_COUNT]  = {KEY_F14, KEY_F16, KEY_F18, KEY_F20, KEY_F22};

// Pressing the encoder button  →  SHIFT + this key
const uint8_t KEY_PRESS[ENCODER_COUNT]      = {KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17};

// ================================================================
// BLE Keyboard
// ================================================================
// Creates a virtual Bluetooth keyboard. Windows/Mac will see it
// as a regular keyboard named "SoundControl2".
// Parameters: device name, manufacturer name, battery level (%)
BleKeyboard bleKeyboard("SoundControl2", "LetsTinker", 100);

// ================================================================
// Encoders
// ================================================================
// Each encoder needs: A-pin, B-pin, button-pin, steps-per-notch
// Pin numbers are defined in pin_def.h
ClickEncoder encoder1(ENC1_A, ENC1_B, ENC1_BTN, ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder2(ENC2_A, ENC2_B, ENC2_BTN, ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder3(ENC3_A, ENC3_B, ENC3_BTN, ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder4(ENC4_A, ENC4_B, ENC4_BTN, ENCODER_STEPS_PER_NOTCH);
ClickEncoder encoder5(ENC5_A, ENC5_B, ENC5_BTN, ENCODER_STEPS_PER_NOTCH);

// Pointer array so we can loop over all encoders easily
ClickEncoder* encoders[ENCODER_COUNT] = {&encoder1, &encoder2, &encoder3, &encoder4, &encoder5};

// Tracks accumulated rotation for each encoder
int16_t encoderValue[ENCODER_COUNT]     = {0};
int16_t encoderLastValue[ENCODER_COUNT] = {0};

// ================================================================
// Pedal Buttons
// ================================================================
DigitalButton pedal1(PED1, false);
DigitalButton pedal2(PED2, false);

// ================================================================
// Helper: send a key combination over BLE
// ================================================================
// Presses a modifier (e.g. CTRL or SHIFT) together with a key,
// then releases both. This is how keyboard shortcuts work.
void sendKey(uint8_t modifier, uint8_t key) {
  bleKeyboard.press(modifier);
  bleKeyboard.press(key);
  bleKeyboard.releaseAll();
}

// ================================================================
// Setup — runs once at startup
// ================================================================
void setup() {
  Serial.begin(115200);
  Serial.println("SoundControl starting...");

  // Enable held and double-click detection for all encoder buttons
  for (int i = 0; i < ENCODER_COUNT; i++) {
    encoders[i]->setButtonHeldEnabled(true);
    encoders[i]->setDoubleClickEnabled(true);
  }

  // Start the BLE keyboard — after this the device appears
  // in Bluetooth settings and can be paired
  bleKeyboard.begin();
  Serial.println("BLE keyboard started, waiting for connection...");
}

// ================================================================
// Loop — runs continuously
// ================================================================
void loop() {

  // --- Pedal service ----------------------------------------------
  // Pedals need to be checked every 1ms for accurate button timing.
  // micros() returns microseconds, so 1000µs = 1ms.
  static uint32_t lastService = 0;
  if (micros() - lastService >= 1000) {
    lastService = micros();
    pedal1.service();
    pedal2.service();
  }

  // --- Encoders ---------------------------------------------------
  for (int i = 0; i < ENCODER_COUNT; i++) {
    encoders[i]->service();

    // getValue() returns how many steps turned since last call.
    // We accumulate them so fast turns aren't missed.
    encoderValue[i] += encoders[i]->getValue();

    if (encoderValue[i] != encoderLastValue[i]) {
      Serial.print("Encoder "); Serial.print(i + 1);
      Serial.print(" value: "); Serial.println(encoderValue[i]);

      if (bleKeyboard.isConnected()) {
        if (encoderValue[i] > encoderLastValue[i])
          sendKey(KEY_LEFT_CTRL, KEY_TURN_RIGHT[i]);
        else
          sendKey(KEY_LEFT_CTRL, KEY_TURN_LEFT[i]);
      }

      encoderLastValue[i] = encoderValue[i];
    }

    // --- Encoder button ---
    ClickEncoder::Button button = encoders[i]->getButton();
    if (button == ClickEncoder::Clicked) {
      Serial.print("Encoder "); Serial.print(i + 1); Serial.println(" clicked");
      if (bleKeyboard.isConnected())
        sendKey(KEY_LEFT_SHIFT, KEY_PRESS[i]);
    }
  }

  // --- Pedal 1 ----------------------------------------------------
  if (pedal1.getButton() == ClickEncoder::Clicked) {
    Serial.println("Pedal 1 clicked");
    if (bleKeyboard.isConnected())
      sendKey(KEY_LEFT_SHIFT, KEY_F19);
  }

  // --- Pedal 2 ----------------------------------------------------
  if (pedal2.getButton() == ClickEncoder::Clicked) {
    Serial.println("Pedal 2 clicked");
    if (bleKeyboard.isConnected())
      sendKey(KEY_LEFT_SHIFT, KEY_F20);
  }

}
