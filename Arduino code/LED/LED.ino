//LED test
//Tests 3 led, switches on/off every 1 second

//Wiring:
//LED - Arduino Nano
//+ve - Resistor - D2
//-ve - GND

const int powerLED = 10;
const int waitingLED = 11;
const int runningLED = 12;

void setup() {
  pinMode(powerLED, OUTPUT);
  pinMode(waitingLED, OUTPUT);
  pinMode(runningLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("POWER LED");
  digitalWrite(powerLED, HIGH);
  digitalWrite(waitingLED, LOW);
  digitalWrite(runningLED, LOW);
  delay(500);

  Serial.println("WAITING LED");
  digitalWrite(powerLED, LOW);
  digitalWrite(waitingLED, HIGH);
  digitalWrite(runningLED, LOW);
  delay(500);

  Serial.println("RUNNING LED");
  digitalWrite(powerLED, LOW);
  digitalWrite(waitingLED, LOW);
  digitalWrite(runningLED, HIGH);
  delay(500);

  Serial.println("OFF");
  digitalWrite(powerLED, LOW);
  digitalWrite(waitingLED, LOW);
  digitalWrite(runningLED, LOW);
  delay(500);
}
