#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

#include <Arduino.h>
#include "AdsrEnvelope.h"

#if defined(ESP32)
#include <ESP32Encoder.h>
#define ENCODER_A_PIN 32
#define ENCODER_B_PIN 33
#define ENCODER_BUTTON_PIN 26
#else
#include <Encoder.h>
#define ENCODER_A_PIN 5
#define ENCODER_B_PIN 6
#define ENCODER_BUTTON_PIN 7

#endif

class EncoderHandler {
    public:
    using OnEncoderChanged = void(*)();
    EncoderHandler(AdsrEnvelope* adsr);
    void setup();

    enum State {
        ATTACK_DURATION,
        ATTACK_SHAPE,
        ENVELOPE_DURATION
    };

    inline State getState() { return encoderState; }
    void tick();
    void registerOnEncoderChange(OnEncoderChanged callback);
    inline State getEncoderState() { return encoderState; }

    private: 
    static EncoderHandler* instance; 
    OnEncoderChanged onEncoderChanged;
    State encoderState;
    long lastButtonClickTime = 0;

    void onPushButtonImpl();
    
#if defined(ESP32)
    ESP32Encoder encoder;
#else
    Encoder encoder(ENCODER_A_PIN, ENCODER_B_PIN);
#endif

    long encoderPosition = 0;

    AdsrEnvelope* adsr;

    #if defined(ESP32)
    static void IRAM_ATTR onPushButton();
    # else
    static void onPushButton();
    #endif
};

#endif