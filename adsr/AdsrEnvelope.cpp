#include <Arduino.h>
#include "AdsrEnvelope.h"

AdsrEnvelope::AdsrEnvelope(double envelopeMax, double attackDurationMs, double decayDurationMs, double sustainDurationMs, 
    double sustainMax, double releaseDurationMs) {
    this->envelopeMax = envelopeMax;
    this->attackDurationMs = attackDurationMs;
    this->decayDurationMs = decayDurationMs;
    this->sustainDurationMs = sustainDurationMs;
    this->sustainMax = sustainMax;
    this->releaseDurationMs = releaseDurationMs;

    this->attackShapeFactor = 5.0;
    this->decayShapeFactor = 10.0;
    this->releaseShapeFactor = 1.0;
}

double AdsrEnvelope::getEnvelopeValue(double time) {
    double envelopeValue = 0.0;
    double timeSinceStart = time - this->envelopeStartTime;
    if (timeSinceStart < this->attackDurationMs) {
        // attack phase
        double phaseTime = timeSinceStart;
        if (attackShapeFactor < 5.0) {
            // linear
            envelopeValue = envelopeMax * (phaseTime / attackDurationMs);
        } else {
            // exponential
            double tau = attackDurationMs / attackShapeFactor;
            envelopeValue = envelopeMax * (1.0 - exp(-phaseTime / tau));
        }
    } else if (timeSinceStart < this->attackDurationMs + this->decayDurationMs) {
        // decay phase
        double phaseTime = timeSinceStart - this->attackDurationMs;
        if (attackShapeFactor < 5.0) {
            // linear
            envelopeValue = sustainMax + (envelopeMax - sustainMax) * (1.0 - phaseTime / decayDurationMs);
        } else {
            // exponential
            double tau = decayDurationMs / decayShapeFactor;
            envelopeValue = sustainMax + (envelopeMax - sustainMax) * exp(-phaseTime / tau);
        }
    } else if (timeSinceStart < this->attackDurationMs + this->decayDurationMs + this->sustainDurationMs) {
        // sustain phase
        envelopeValue = this->sustainMax;
    } else if (timeSinceStart < 
            this->attackDurationMs + this->decayDurationMs + this->sustainDurationMs + this->releaseDurationMs) {
        // release phase
        double phaseTime = timeSinceStart - this->attackDurationMs - this->decayDurationMs - this->sustainDurationMs;
        if (releaseShapeFactor < 6.0) {
            // linear
            envelopeValue = this->sustainMax * (1.0 - phaseTime / releaseDurationMs);
        } else {
            // exponential
            double tau = releaseDurationMs / releaseShapeFactor;
            envelopeValue = this->sustainMax * exp(-phaseTime / tau);
        }
    } else {
        envelopeValue = 0.0;
    }
    return envelopeValue;
}

