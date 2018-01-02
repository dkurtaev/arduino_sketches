const uint8_t kTriggerPin = 9;
const uint8_t kEchoPin = 8;

void setup() {
  Serial.begin(9600);
  pinMode(kTriggerPin, OUTPUT);
  pinMode(kEchoPin, INPUT);
  digitalWrite(kTriggerPin, LOW);
}

void loop() {
  digitalWrite(kTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(kTriggerPin, LOW);

  const int duration = pulseIn(kEchoPin, HIGH);
  Serial.println(duration);

  delay(100);
}
