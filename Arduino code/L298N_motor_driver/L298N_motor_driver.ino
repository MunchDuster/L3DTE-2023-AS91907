//L298N Dual H-Bridge motor driver
//Double motor test
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

const int IN1 = 9;
const int IN2 = 8;
const int IN3 = 7;
const int IN4 = 6;

void setup() {
  //Set motor controls as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  Stop();
  delay(1000);
  Forward();
  delay(1000);
  Stop();
  delay(1000);
  Backward();
  delay(1000);
}

void Stop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void Forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void Backward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}