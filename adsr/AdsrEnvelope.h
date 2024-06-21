#ifndef ADSR_ENVELOPE_H
#define ADSR_ENVELOPE_H

class AdsrEnvelope {
private:
    double envelopeMax;
    double attackShapeFactor;
    double attackDurationMs;
    double envelopeStartTime;
    double decayDurationMs;
    double decayShapeFactor;
    double sustainDurationMs;
    double sustainMax;
    double releaseDurationMs;
    double releaseShapeFactor;

public:
    AdsrEnvelope(double envelopeMax, double attackDurationMs, double decayDurationMs, double sustainDurationMs, 
        double sustainMax, double releaseDurationMs);
    
    inline void setEnvelopeStartTime(double time) { this->envelopeStartTime = time; }

    double getEnvelopeValue(double time);

    inline void setEnvelopeMax(double envelopeMax) { this->envelopeMax = envelopeMax; }
    inline void setAttackShapeFactor(double attackShapeFactor) { this->attackShapeFactor = attackShapeFactor; }
    inline void setAttackDurationMs(double attackDurationMs) { this->attackDurationMs = attackDurationMs; }
    inline void setdecayDurationMs(double decayDurationMs) { this->decayDurationMs = decayDurationMs; }
    inline void setdecayShapeFactor(double decayShapeFactor) { this->decayShapeFactor = decayShapeFactor; }
    inline void setSustainDurationMs(double sustainDurationMs) { this->sustainDurationMs = sustainDurationMs; }
    inline void setReleaseDurationMs(double releaseDurationMs) { this->releaseDurationMs = releaseDurationMs; }
    inline void setReleaseShapeFactor(double releaseShapeFactor) { this->releaseShapeFactor = releaseShapeFactor; }
    void setEnvelopeDurationMs(double envelopeDurationMs);

    inline double getEnvelopeStartTime() { return envelopeStartTime; }
    inline double getEnvelopeMax() { return envelopeMax; }
    inline double getAttackShapeFactor() { return attackShapeFactor; }
    inline double getAttackDurationMs() { return attackDurationMs; }
    inline double getDecayDurationMs() { return decayDurationMs; }
    inline double getDecayShapeFactor() { return decayShapeFactor; }
    inline double getSustainDurationMs() { return sustainDurationMs; }
    inline double getSustainMax() { return sustainMax; }
    inline double getEnvelopeDurationMs() { return attackDurationMs + decayDurationMs + sustainDurationMs + releaseDurationMs; }
};
#endif