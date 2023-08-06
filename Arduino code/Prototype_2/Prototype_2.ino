//Prototype 1
//Parts: L298N, LEDs, Buttons, (Later) Accelerometer
//Not included: MPU6050 acceleromter and gyro
  
//Wiring:
//L298N - Arduino Nano//
//GND - GND
//+5V - Vin
//IN1 - D10
//IN2 - D9
//IN3 - D8
//IN4 - D7
//Piezo buzzer - Arduino//
//(-) - GND
//(+) - 100 ohm resistor - D11

#include <List.hpp>

//Button input pins
const int forwardButton = 6;
const int rightButton = 5;
const int leftButton = 4;
const int backButton = 3;
const int startButton = 2;

//LED output pins
const int runningLED = A2;
const int waitingLED = A1;
const int powerLED = A0;

//Motor output pins
const int IN1 = 10;
const int IN2 = 9;
const int IN3 = 8;
const int IN4 = 7;

//Piezo buzzer pin
const int buzzer = 11;

//Control settings
const int TURN_TIME = 300; //Should turn about 90 degrees
const int DRIVE_TIME = 600; //Should drive about 20 centimeters
const int BUTTON_CHECK_DELAY = 20; //50 Hz refresh rate
const int FINISHED_GAP = 1000;
const int INSTRUCTION_GAP = 400;
const int START_RUNNING_GAP = 600;

//Piezo buzzer song settings    1, 5, 8, 12, 17, 12, 8, 5
//Song array order: 0 is number of notes in song, 1 is noteNumber, 2 is noteDuration, 3 is noteGap (repeat 1,2,3)
int songArray[][] = {
  /*Startup */ {3, 0, 200, 300, 0, 200, 300, 0, 200, 300},
  /*Input   */ {2, 0, 100, 30, 8, 100, 30},
  /*Running */ {4, 5, 400, 200, 5, 400, 200, 12, 400, 200, 17, 400, 200},
  /*Finished*/ {4, 12, 200, 100, 8, 200, 100, 5, 200, 100, 0, 200, 100},
}

//Variables for storing instructions
enum Instruction { FORWARD, BACKWARD, LEFT, RIGHT } ;
List<Instruction> instructions;

//Variables for button inputs
Instruction pendingInstruction = NULL;
Instruction lastInstruction = NULL;
bool pendingStart;

void setup() {
  Serial.begin(9600);
  
  //Set motor controls as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //Set LEDs as output
  pinMode(runningLED, OUTPUT);
  pinMode(powerLED, OUTPUT);
  pinMode(waitingLED, OUTPUT);

  //Set buttons as input pullups
  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);

  //Turn power LED on
  digitalWrite(powerLED, HIGH);

  //Play starting song
  playSong(0);
}

void loop() {
  //Waiting
  digitalWrite(runningLED, LOW);
  digitalWrite(waitingLED, HIGH);
  GetInput();
  
  //Running
  //Play running song
  playSong(2);
  delay(START_RUNNING_GAP);
  digitalWrite(waitingLED, LOW);
  digitalWrite(runningLED, HIGH);
  RunInstructions();
  Serial.println("FINISHED RUNNING");
  //Play finished song
  playSong(3);
  delay(FINISHED_GAP);
}

///MAIN FUNCTIONS///
void GetInput() {  
  Serial.println("GETTING INPUT");
  while(!pendingStart) {
    //Get input
    CheckButtons();
    //Add input to instructions
    if(pendingInstruction != NULL) {
      instructions.add(pendingInstruction);
      lastInstruction = pendingInstruction;
      pendingInstruction = NULL;
    }
    delay(BUTTON_CHECK_DELAY);
  }
  pendingStart = false;
  pendingInstruction = NULL;
}
void RunInstructions() {
  for(int i = 0; i < instructions.getSize(); i++) {
    switch(instructions.getValue(i)) {
      case FORWARD: Drive(true); break;
      case BACKWARD: Drive(false); break;
      case LEFT: Turn(true); break;
      case RIGHT: Turn(false); break;
    }
    Stop();
    delay(INSTRUCTION_GAP);
  }
  instructions.clear();
}

///INPUT BUTTONS///
void CheckButtons() {
  //Wait for last button to be released
  if(lastInstruction != NULL) {
    int checkButton;
    switch(lastInstruction) {
      case FORWARD: checkButton = forwardButton; break;
      case BACKWARD: checkButton = backButton; break;
      case LEFT: checkButton = leftButton; break;
      case RIGHT: checkButton = rightButton; break;
    }
    //Check the button is not being pressed
    if(digitalRead(checkButton) == HIGH) {
      lastInstruction = NULL;
    }
    else return;
  }
  //Because buttons are pullup, LOW is when pressed
  if(digitalRead(forwardButton) == LOW) {pendingInstruction = FORWARD; Serial.println("FORWARD PRESSED"); }
  else if(digitalRead(backButton) == LOW) {pendingInstruction = BACKWARD; Serial.println("BACKWARD PRESSED"); }
  else if(digitalRead(leftButton) == LOW) {pendingInstruction = LEFT; Serial.println("LEFT PRESSED"); }
  else if(digitalRead(rightButton) == LOW) {pendingInstruction = RIGHT; Serial.println("RIGHT PRESSED"); }
  else if(digitalRead(startButton) == LOW) {pendingStart = true; Serial.println("START PRESSED"); }

  //Play input song if button pressed
  if(pendingInstruction != NULL) playSong(1);
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
///PIEZO BUZZER FUNCTIONS///
void playSong(int songIndex) {
  int notesInSong = songs[songIndex][0];
  for(int noteIndex = 0; noteIndex < notesInSong; noteIndex++) {
    int arrayIndex = noteIndex * 3 + 1;
    playNote(songs[songIndex][arrayIndex], songs[songIndex][arrayIndex + 1], songs[songIndex][arrayIndex + 2]
  }
}
const int magicNumber = pow(2.0, 1.0/12.0);
void playNote(int noteNumber, int noteDuration, int noteGap) {
  double frequency = 440.0 * pow(magicNumber, noteNumber); // frequency of note in half steps from A4
    tone(buzzer, (int)frequency); //Play the sound to the nearest integer frequency
    delay(noteDuration);
    noTone(buzzer); //Quiet
    delay(noteGap);
}
