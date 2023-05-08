//L298N Dual H-Bridge motor driver
//Single motor test
//No control for motor speed (only direction and on/off) because not using pin ENA

//Wiring:
//L298N - Arduino Nano
//GND - GND
//+5V - 5V
//IN1 - D9
//IN2 - D8
//External battery (Used 9V)
//+ve - +12V (L298N)
//-ve - GND (Arduino)

#define IN1 9
#define IN2 8

void setup() {
  // Set motor connections as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Start with motor off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  // Set motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(1000);

  // Set motor reverse
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(1000);
}
