
#define leftButtonPin 4
#define rightButtonPin 5
#define upButtonPin 3
#define downButtonPin 6

void setup() {
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  bool rightValue = !digitalRead(rightButtonPin);
  bool leftValue = !digitalRead(leftButtonPin);
  bool upValue = !digitalRead(upButtonPin);
  bool downValue = !digitalRead(downButtonPin);
  Serial.print(rightValue);
  Serial.print(leftValue);
  Serial.print(upValue);
  Serial.println(downValue);
  delay(10);
}
