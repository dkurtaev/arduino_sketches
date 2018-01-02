const int FIRST_LED_PIN = 5;
const int NUM_LEDS = 8;
// Analog pin A0 which is connected to microphone's output receives just a binary
// signals. To measure a volume of sound we need to make several reads
// (see pulse-width modulation).
const int NUM_PIN_READS = 200;

void setup() {
  pinMode(A0, INPUT);

  for (int i = 0; i < NUM_LEDS; ++i)
    pinMode(FIRST_LED_PIN + i, OUTPUT);
}

// There is some delay between an actual sound volume and displayed one.
int top = 0;

void loop() {
    // Estimete a sound.
    float n = 0;
    for (int i = 0; i < NUM_PIN_READS; ++i) {
      // analogRead(A0) returns 1023 or some small number close to 0 here. So
      // we use value 500 to say if sound is detected.
      n += analogRead(A0) < 500;
    }
    // Map to [0, 1]
    n /= NUM_PIN_READS;

    // Compute how many LEDs should be turned on.
    int i;
    for (i = 0; i < NUM_LEDS; ++i) {
      float bound = float(i + 1) / NUM_LEDS;
      // It's just a heuristic function.
      if (n < 0.5 * bound * bound) {
        break;
      }
    }

    if (i != top) {
      // We make top number of LEDs increase/decrease smother.
      top = max(0, min(top + (i > top ? 1 : -1), NUM_LEDS));
      for (i = 0; i < top; ++i) {
        digitalWrite(FIRST_LED_PIN + i, HIGH);
      }
      for (; i < NUM_LEDS; ++i) {
        digitalWrite(FIRST_LED_PIN + i, LOW);
      }
    }
    delay(40);
}
