#ifndef ADSR_ENVELOPE_H

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

    inline double getEnvelopeDurationMs() { return attackDurationMs + decayDurationMs + sustainDurationMs + releaseDurationMs; }
};
#endif