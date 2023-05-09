//LED test
//Tests 1 led, switches on/off every 1 second

//Wiring:
//LED - Arduino Nano
//+ve - Resistor - D8
//-ve - GND

#define waitTime 1000
#define ledPin 8

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(waitTime);
  digitalWrite(ledPin, LOW);
  delay(waitTime);
}
