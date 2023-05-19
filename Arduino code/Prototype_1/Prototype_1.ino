//Prototype 1

//All four motor test
//No control for motor speed (only direction and on/off) because not using pin ENA or ENB
//No sensors

//Wiring:
//L298N (Left) - Arduino Nano
//GND - GND
//+5V - 5V
//IN1 - D9
//IN2 - D8
//L298N (Right) - Arduino Nano
//GND - GND
//+5V - 5V
//IN1 - D7
//IN2 - D6
//External battery (Right) (Used 9V)
//+ve - +12V (L298N Right)
//-ve - GND (Arduino)
//External battery (Left) (Used 9V)
//+ve - +12V (L298N Left)
//-ve - GND (Arduino)

#define waitTime 4000
#define motorDriverLeftIn1 9
#define motorDriverLeftIn2 8
#define motorDriverRightIn1 7
#define motorDriverRightIn2 6

void setup() {
  //Setup motor driver pins output
  pinMode(motorDriverLeftIn1, OUTPUT);
  pinMode(motorDriverLeftIn2, OUTPUT);
  pinMode(motorDriverRightIn1, OUTPUT);
  pinMode(motorDriverRightIn2, OUTPUT);

  // Start with motors off
  Stop();

  //Start testing
  Forward(true);
  delay(waitTime);

  Forward(false);
  delay(waitTime);

  Turn(false);
  delay(waitTime);

  Turn(true)
  delay(waitTime);

  //Testing finished
  Stop();
}

void loop() { }

void Turn(bool left)
{
  //Left
  digitalWrite(motorDriverLeftIn1, left);
  digitalWrite(motorDriverLeftIn2, !left);

  //Right (opposite of left)
  digitalWrite(motorDriverRightIn1, !left);
  digitalWrite(motorDriverRightIn2, left);
}
void Drive(bool forward)
{
  //Left
  digitalWrite(motorDriverLeftIn1, left);
  digitalWrite(motorDriverLeftIn2, !left);

  //Right (same as left)
  digitalWrite(motorDriverRightIn1, left);
  digitalWrite(motorDriverRightIn2, !left);
}
void Stop()
{
  //Left
  digitalWrite(motorDriverLeftIn1, false);
  digitalWrite(motorDriverLeftIn2, false);

  //Right
  digitalWrite(motorDriverRightIn1, false);
  digitalWrite(motorDriverRightIn2, false);
}