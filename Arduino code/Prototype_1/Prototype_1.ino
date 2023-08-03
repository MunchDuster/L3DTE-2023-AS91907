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
const int forwardButton = A0;
//const int rightButton = A4;//not used in this deliverable
const int leftButton = A2;
const int backButton = A3;
const int startButton = A1;

//LED output pins
//const int runningLED = 2;
//const int waitingLED = 3;
//const int finishedLED = 4;
const int powerLED = A5;

//Motor output pins
const int IN1 = 9;
const int IN2 = 8;
const int IN3 = 7;
const int IN4 = 6;
const int ENA = 5;
const int ENB = 4;

//Control settings
const int MOTOR_SPEED_BYTE = 130;//Values lower than this won't work for unknown reasons
const int TURN_TIME = 300; //Should turn about 90 degrees
const int DRIVE_TIME = 600; //Should drive about 20 centimeters
const int ACCELERATION_TIME = 2000;
const int DECELERATION_TIME = 2000;
const int BUTTON_CHECK_DELAY = 20; //50 Hz refresh rate
const int FINISHED_DELAY = 1000;
const int INSTRUCTION_GAP = 400;
const int START_RUNNING_GAP = 600;

//Variables for storing instructions
enum Instruction { FORWARD, BACKWARD, LEFT, RIGHT, EMPTY } ;
List<Instruction> instructions;

//Variables for button inputs
Instruction pendingInstruction = EMPTY;
Instruction lastInstruction = EMPTY;
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

  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  //Set LEDs as output
//  pinMode(runningLED, OUTPUT);
  pinMode(powerLED, OUTPUT);
//  pinMode(waitingLED, OUTPUT);
//  pinMode(finishedLED, OUTPUT);

  //Set buttons as input pullups
  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
//  pinMode(rightButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);

  //Turn power LED on
  digitalWrite(powerLED, HIGH);
}

void loop() {
  //Waiting
//  digitalWrite(finishedLED, LOW);
//  digitalWrite(waitingLED, HIGH);
  GetInput();
  delay(1000);
  
  
  //Running
//  digitalWrite(waitingLED, LOW);
//  digitalWrite(runningLED, HIGH);
  delay(START_RUNNING_GAP);
  RunInstructions();

  //Finished
  Serial.println("FINISHED RUNNING... RESTARTING");
//  digitalWrite(runningLED, LOW);
//  digitalWrite(finishedLED, HIGH);
  delay(FINISHED_DELAY);
  
}

///MAIN FUNCTIONS///
void GetInput() {  
  Serial.println("GETTING INPUT");
  while(!pendingStart) {
    //Get input
    CheckButtons();
    //Add input to instructions
    if(pendingInstruction != EMPTY) {
      instructions.add(pendingInstruction);
      lastInstruction = pendingInstruction;
      pendingInstruction = EMPTY;
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
  //Wait for last button to be released
  if(lastInstruction != EMPTY) {
    int checkButton;
    switch(lastInstruction) {
      case FORWARD: checkButton = forwardButton; break;
      case BACKWARD: checkButton = backButton; break;
      case LEFT: checkButton = leftButton; break;
//      case RIGHT: checkButton = rightButton; break;
    }
    //Check the button is not being pressed
    if(digitalRead(checkButton) == HIGH) {
      lastInstruction = EMPTY;
    }
    else return;
    
  }
  //Because buttons are pullup, LOW is when pressed
  if(digitalRead(forwardButton) == LOW) {pendingInstruction = FORWARD; Serial.println("FORWARD PRESSED"); }
  else if(digitalRead(backButton) == LOW) {pendingInstruction = BACKWARD; Serial.println("BACKWARD PRESSED"); }
  else if(digitalRead(leftButton) == LOW) {pendingInstruction = LEFT; Serial.println("LEFT PRESSED"); }
//  else if(digitalRead(rightButton) == LOW) {pendingInstruction = RIGHT; Serial.println("RIGHT PRESSED"); }
  else if(digitalRead(startButton) == LOW) {pendingStart = true; Serial.println("START PRESSED"); }
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void Drive(bool forward) {
  Serial.print("Driving ");
  Serial.println(forward ? " forward" : " backward");
  digitalWrite(IN1, forward);
  digitalWrite(IN2, !forward);
  digitalWrite(IN3, forward);
  digitalWrite(IN4, !forward);
  delay(DRIVE_TIME);
}
void Turn(bool left) {
  Serial.print("Turning ");
  Serial.println(left ? " left" : " right");
  digitalWrite(IN1, left);
  digitalWrite(IN2, !left);
  digitalWrite(IN3, !left);
  digitalWrite(IN4, left);
  delay(TURN_TIME);
}
