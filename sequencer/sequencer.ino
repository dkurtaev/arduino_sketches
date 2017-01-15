const int TRUE_ALARM_PIN = 9;
const int FALSE_ALARM_PIN = 8;
const int MAX_SEQ_LENGTH = 7;

int game_lights_pins[] = {12, 11, 10};
bool button_states[] = {LOW, LOW, LOW};
int sequence[MAX_SEQ_LENGTH];
int sequence_length = 0;
int received_sequence_length = 0;

void showSequence();
void increaseSequence();
bool buttonPressed(int pin);

void setup() {
  pinMode(TRUE_ALARM_PIN, OUTPUT);
  pinMode(FALSE_ALARM_PIN, OUTPUT);
  randomSeed(analogRead(0));
  delay(1000);

  increaseSequence();
  showSequence();
}

void loop() {
  if (received_sequence_length != MAX_SEQ_LENGTH) {
    if (button_states[0] == LOW &&
        button_states[1] == LOW &&
        button_states[2] == LOW) {
      // Receive next signal.
      for (int i = 0; i < 3; ++i) {
        if (buttonPressed(game_lights_pins[i])) {
          button_states[i] = HIGH;
          if (i == sequence[received_sequence_length]) {
            received_sequence_length += 1;
            // If sequence repeated completely.
            if (received_sequence_length == sequence_length) {
              digitalWrite(TRUE_ALARM_PIN, HIGH);
              delay(1000);
              if (received_sequence_length != MAX_SEQ_LENGTH) {
                digitalWrite(TRUE_ALARM_PIN, LOW);
                increaseSequence();
                received_sequence_length = 0;
                showSequence();
              }
            }
          } else {
            digitalWrite(FALSE_ALARM_PIN, HIGH);
            delay(1000);
            digitalWrite(FALSE_ALARM_PIN, LOW);
            received_sequence_length = 0;
            showSequence();
          }
          break;
        }
      }
    } else {
      for (int i = 0; i < 3; ++i) {
        button_states[i] = buttonPressed(game_lights_pins[i]);
      }
    }
  } else {
    showSequence();
  }
}

void showSequence() {
  int delay_time = 500 + 500 * (1.0f - (float)sequence_length / MAX_SEQ_LENGTH);
  for (int i = 0; i < 3; ++i) {
    pinMode(game_lights_pins[i], OUTPUT);
  }
  for (int i = 0; i < sequence_length; ++i) {
    digitalWrite(game_lights_pins[sequence[i]], HIGH);
    delay(delay_time);
    digitalWrite(game_lights_pins[sequence[i]], LOW);
    delay(delay_time / 2);
  }
  for (int i = 0; i < 3; ++i) {
    pinMode(game_lights_pins[i], INPUT);
  }
}

void increaseSequence() {
  if (sequence_length < MAX_SEQ_LENGTH) {
    sequence[sequence_length] = random(0, 3);
    sequence_length += 1;
  }
}

bool receiveNext(int id) {
  if (id == sequence[received_sequence_length]) {
    received_sequence_length += 1;
    return true;
  } else {
    return false;
  }
}

bool buttonPressed(int pin) {
  static const int kCheckDelay = 5;
  if (digitalRead(pin) == HIGH) {
    delay(kCheckDelay);
    return digitalRead(pin) == HIGH;
  } else {
    return false;
  }
}
