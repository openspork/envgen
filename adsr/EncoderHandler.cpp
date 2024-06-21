#include "EncoderHandler.h"

EncoderHandler* EncoderHandler::instance = nullptr;  

EncoderHandler::EncoderHandler(AdsrEnvelope* adsr) : adsr(adsr), encoderState(ATTACK_DURATION) {
    instance = this;
}

void EncoderHandler::setup() {
    pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
#if defined(ESP32)
    encoder.attachHalfQuad(ENCODER_A_PIN, ENCODER_B_PIN);
#endif
    attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON_PIN), onPushButton, RISING);
}

void EncoderHandler::registerOnEncoderChange(void (*callback)()) {
    onEncoderChanged = callback;
}

void EncoderHandler::tick() {
#if defined(ESP32)
    long newEncoderPosition = encoder.getCount() / 2;
#else
    long newEncoderPosition = encoder.read() / 4;
#endif

    if (newEncoderPosition != encoderPosition) {
        Serial.print("Encoder position: ");
        Serial.println(newEncoderPosition);

        long encoderDelta = (newEncoderPosition - encoderPosition);
        encoderPosition = newEncoderPosition;
        double attackDurationMs;
        double attackShapeFactor;
        switch (encoderState) {
        case ATTACK_DURATION:
            attackDurationMs = adsr->getAttackDurationMs();
            attackDurationMs += 50 * encoderDelta;
            if (attackDurationMs > 1000) {
                attackDurationMs = 1000;
            } else if (attackDurationMs < 50) {
                attackDurationMs = 50;
            }
            adsr->setAttackDurationMs(attackDurationMs);
            Serial.print("Attack duration: ");
            Serial.println(attackDurationMs);
            break;
        case ATTACK_SHAPE:
            attackShapeFactor = adsr->getAttackShapeFactor();
            attackShapeFactor += 0.5 * encoderDelta;
            if (attackShapeFactor > 10) {
                attackShapeFactor = 10;
            } else if (attackShapeFactor < 4) {
                attackShapeFactor = 4;
            }
            adsr->setAttackShapeFactor(attackShapeFactor);
            Serial.print("Attack shape factor: ");
            Serial.println(attackShapeFactor);
            break;
        }
        if (onEncoderChanged) {
            onEncoderChanged();
        }
   }
}

#if defined(ESP32)
void IRAM_ATTR EncoderHandler::onPushButton() {
# else
void EncoderHandler::onPushButton() {
#endif
  // note: not okay to call Serial.println method on ESP32 here as it blocks.
  // TODO: probably need to debounce.
  instance->encoderState = (EncoderHandler::State)((instance->encoderState + 1) % 2);
}