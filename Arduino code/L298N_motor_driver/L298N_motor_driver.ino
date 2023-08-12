//L298N Dual H-Bridge motor driver
//Double motor test
//No control for motor speed (only direction and on/off) because not using pin ENA or ENB

//Wiring:
//L298N - Arduino Nano
//GND - GND
//IN4 - A3
//IN3 - A2
//IN2 - A1
//IN1 - A0
//External battery (Used 9V)
//+ve - +12V (L298N)
//-ve - GND (Arduino)

const int IN1 = A0;
const int IN2 = A1;
const int IN3 = A2;
const int IN4 = A3;

enum Instruction {FORWARD,BACKWARD,LEFT,RIGHT};

Instruction instructions[] = {
  FORWARD,
  LEFT,
  FORWARD,
  LEFT,
  FORWARD,
  LEFT,
  FORWARD,
  LEFT
};

int instructionCount = 8;

void setup() {
  Serial.begin(9600);
  
  //Set motor controls as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  delay(4000);
  for(int i = 0; i < instructionCount; i++) {
    switch(instructions[i]){
      case FORWARD:
        Drive(true);
        break;
      case BACKWARD:
        Drive(false);
        break;
      case LEFT:
        Turn(true);
        break;
      case RIGHT:
        Turn(false);
        break;
    }
    delay(200);
    Stop();
    delay(1000);
  }
}

void Stop(){
  Serial.println("Stopping");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void Drive(bool forward){
  Serial.println("Driving: " + forward);
  digitalWrite(IN1, forward);
  digitalWrite(IN2, !forward);
  digitalWrite(IN3, forward);
  digitalWrite(IN4, !forward);
}
void Turn(bool left){
  Serial.println("Turning: " + left);
  digitalWrite(IN1, left);
  digitalWrite(IN2, !left);
  digitalWrite(IN3, !left);
  digitalWrite(IN4, left);
}
