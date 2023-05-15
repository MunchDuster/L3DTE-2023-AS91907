//LED test
//Tests 1 led, switches on/off every 1 second

//Wiring:
//LED - Arduino Nano
//+ve - Resistor - D2
//-ve - GND

#define waitTime 1000
#define ledPin 2

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(waitTime);
  digitalWrite(ledPin, LOW);
  delay(waitTime);
}
