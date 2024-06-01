int minEnvelope = 500;
float envelope = minEnvelope;

float newEnvelope;

// Phases
int phase = 1;

// Attack = 0
float attackRate = .9;
int attackMax = 4095;

// Decay = 1
float decayRate = .9;
int decayMin = 3000;

// Sustain = 2
int sustainLen = 30000;

// Release = 3
float releaseRate = .9;

const float ATTACK_SECONDS = 3.0;

void setup() {
  analogWriteResolution(12);  // Max out DAC resolution
  Serial.begin(9600);
}

void loop() {
  
  while (phase == 0) {

    // Set asymptote +1 above desired max to hackily cut off curve
    newEnvelope = (1.0 - attackRate) * (attackMax + 1) + attackRate * envelope;
    envelope = newEnvelope;
    analogWrite(DAC, envelope);
    if (newEnvelope >= attackMax) {
      phase++;
    }
  }

  while (phase == 1) {
    newEnvelope = (1.0 - decayRate) * (decayMin - 1) + decayRate * envelope;
    envelope = newEnvelope;
    analogWrite(DAC, envelope);
    if (newEnvelope <= decayMin) {
      phase++;
    }
  }

  int sustainCnt = sustainLen;
  while (phase == 2) {
    sustainCnt--;
    if (sustainCnt == 0) {
      phase++;
    }
  }

  while (phase == 3) {
    newEnvelope = (1.0 - releaseRate) * (minEnvelope - 1) + decayRate * envelope;
    envelope = newEnvelope;
    analogWrite(DAC, envelope);
    if (newEnvelope <= minEnvelope) {
      phase = 0;
    }    
  }
}