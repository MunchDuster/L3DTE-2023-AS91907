//Line trace module (KY-033) test

//WIRING
//Line trace - Arduino Nano
//G - GND
//V+ - 5V
//S - D8

#define lineTracePin 8

void setup() {
  Serial.begin(9600);
  pinMode(lineTracePin, INPUT);
}

void loop() {
  bool sensorValue = digitalRead(lineTracePin);
  Serial.print(sensorValue);
  delay(100);
}
