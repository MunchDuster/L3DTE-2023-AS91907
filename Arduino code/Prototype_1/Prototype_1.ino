//Prototype 1
//Parts: L298N, LEDs, Buttons, (Later) Accelerometer
//Not included: MPU6050 acceleromter and gyro

//Wiring:
//L298N - Arduino Nano//
//GND - GND
//+5V - Vin
//IN1 - D9
//IN2 - D8
//IN3 - D7
//IN4 - D6
//ENA - A1
//ENB - A0

#include <List.hpp>

//Button input pins
const int forwardButton = 10;
const int rightButton = 11;
const int leftButton = 12;
const int backButton = A2;
const int startButton = A3;

//LED output pins
const int runningLED = 2;
const int waitingLED = 3;
const int finishedLED = 4;
const int powerLED = 5;

//Motor output pins
const int IN1 = 9;
const int IN2 = 8;
const int IN3 = 7;
const int IN4 = 6;
const int ENB = A0;
const int ENA = A1;

//Control settings
const int MOTOR_SPEED_BYTE = 130;//Values lower than this won't work for unknown reasons
const int TURN_TIME = 100; //Should turn about 90 degrees
const int DRIVE_TIME = 1000; //Should drive about 20 centimeters
const int ACCELERATION_TIME = 2000;
const int DECELERATION_TIME = 2000;
const int BUTTON_CHECK_DELAY = 20; //50 Hz refresh rate
const int FINISHED_DELAY = 2000;
const int INSTRUCTION_GAP = 800;
const int START_RUNNING_GAP = 800;

//Variables for storing instructions
enum Instruction { FORWARD, BACKWARD, LEFT, RIGHT } ;
List<Instruction> instructions;

//Variables for button inputs
Instruction pendingInstruction;
bool pendingStart;

void setup() {
  Serial.begin(9600);
  
  //Set motor controls as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //Set LEDs as output
  pinMode(runningLED, OUTPUT);
  pinMode(powerLED, OUTPUT);
  pinMode(waitingLED, OUTPUT);
  pinMode(finishedLED, OUTPUT);

  //Set buttons as input pullups
  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);

  //Turn power LED on
  digitalWrite(powerLED, HIGH);
}

void loop() {
  //Waiting
  digitalWrite(finishedLED, LOW);
  digitalWrite(waitingLED, HIGH);
  GetInput();
  
  //Running
  digitalWrite(waitingLED, LOW);
  digitalWrite(runningLED, HIGH);
  delay(START_RUNNING_GAP);
  RunInstructions();

  //Finished
  digitalWrite(runningLED, LOW);
  digitalWrite(finishedLED, HIGH);
  delay(FINISHED_DELAY);
}

///MAIN FUNCTIONS///
void GetInput() {  
  while(!pendingStart) {
    //Get input
    CheckButtons();
    //Apply input
    if(pendingInstruction != NULL) {
      instructions.add(pendingInstruction);
      pendingInstruction = NULL;
    }
    delay(BUTTON_CHECK_DELAY);
  }
  pendingStart = false;
  pendingInstruction = NULL;
}
void RunInstructions() {
  for(int i = 0; i < instructions.getSize(); i++) {
    RunInstruction(instructions.getValue(i));
    Stop();
    delay(INSTRUCTION_GAP);
  }
  instructions.clear();
}

///INPUT BUTTONS///
void CheckButtons() {
  //Because buttons are pullup, LOW is when pressed
  if(digitalRead(forwardButton) == LOW) pendingInstruction = FORWARD;
  else if(digitalRead(backButton) == LOW) pendingInstruction = BACKWARD;
  else if(digitalRead(leftButton) == LOW) pendingInstruction = LEFT;
  else if(digitalRead(rightButton) == LOW) pendingInstruction = RIGHT;
  else if(digitalRead(startButton) == LOW) pendingStart = true; 
}

///INSTRUCTION INTERPRETATION///
void RunInstruction(Instruction instruction) {
  switch(instruction) {
    case FORWARD: Drive(true); break;
    case BACKWARD: Drive(false); break;
    case LEFT: Turn(true); break;
    case RIGHT: Turn(false); break;
  }
}

///SPEED CONTROL FUNCTIONS///
void Accelerate() {
  Serial.println("Accelerating");
  for(int speedByte = 0; speedByte < 255; speedByte++) {
    analogWrite(ENA, speedByte);
    analogWrite(ENB, speedByte);
    delay(ACCELERATION_TIME/255);
  }
}
void Decelerate() {
  Serial.println("Decelerating");
  for(int speedByte = 255; speedByte >= 0; speedByte--) {
    analogWrite(ENA, speedByte);
    analogWrite(ENB, speedByte);
    delay(DECELERATION_TIME/255);
  }
}

///MOTOR CONTROL FUNCTIONS///
void Stop() {
  Serial.println("Stopping");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void Drive(bool forward) {
  Serial.println("Driving: " + forward);
  digitalWrite(IN1, forward);
  digitalWrite(IN2, !forward);
  digitalWrite(IN3, forward);
  digitalWrite(IN4, !forward);
  delay(DRIVE_TIME);
}
void Turn(bool left) {
  Serial.println("Turning: " + left);
  digitalWrite(IN1, left);
  digitalWrite(IN2, !left);
  digitalWrite(IN3, !left);
  digitalWrite(IN4, left);
  delay(TURN_TIME);
}
