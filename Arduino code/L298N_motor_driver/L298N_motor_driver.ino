//L298N Dual H-Bridge motor driver
//Single motor test
//No control for motor speed (only direction and on/off) because not using pin ENA or ENB

//Wiring:
//L298N - Arduino Nano
//GND - GND
//+5V - 5V
//IN1 - D9
//IN2 - D8
//External battery (Used 9V)
//+ve - +12V (L298N)
//-ve - GND (Arduino)

#define waitTime 1000
#define IN1 9
#define IN2 8

bool motor1On = false;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Start with motor off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  digitalWrite(IN1, motor1On);
  digitalWrite(IN2, !motor1On);
  delay(1000);
  motor1On = !motor1On;
}
