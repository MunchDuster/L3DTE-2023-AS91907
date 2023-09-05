//LED test
//Tests 3 led, switches on/off every 1 second

//Wiring:
//LED - Arduino Nano
//+ve - Resistor - D2
//-ve - GND

const int powerLED = 5;
const int waitingLED = 6;
const int runningLED = 7;

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
  delay(2000);

  Serial.println("WAITING LED");
  digitalWrite(powerLED, LOW);
  digitalWrite(waitingLED, HIGH);
  digitalWrite(runningLED, LOW);
  delay(2000);

  Serial.println("RUNNING LED");
  digitalWrite(powerLED, LOW);
  digitalWrite(waitingLED, LOW);
  digitalWrite(runningLED, HIGH);
  delay(2000);
}
