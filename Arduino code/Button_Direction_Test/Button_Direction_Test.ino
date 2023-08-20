//Five button test


#include <List.hpp>

const int BUTTON_CHECK_DELAY = 30;

const int leftButton = 3;
const int startButton = 4;
const int rightButton = 5;
const int forwardButton = 6;
const int backButton = 7;

//Variables for storing instructions
enum Instruction { EMPTY, FORWARD, BACKWARD, LEFT, RIGHT } ;
List<Instruction> instructions;

//Variables for button inputs
Instruction pendingInstruction = EMPTY;
Instruction lastInstruction = EMPTY;
bool pendingStart;

void setup() {
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  GetInput();
  Serial.println("RUNNING (NOT REALLY)");
  delay(1000);
}

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
  pendingInstruction = EMPTY;
}

void CheckButtons() {
  //Wait for last button to be released
  if(lastInstruction != EMPTY) {
    int checkButton;
    switch(lastInstruction) {
      case FORWARD: checkButton = forwardButton; break;
      case BACKWARD: checkButton = backButton; break;
      case LEFT: checkButton = leftButton; break;
      case RIGHT: checkButton = rightButton; break;
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
  else if(digitalRead(rightButton) == LOW) {pendingInstruction = RIGHT; Serial.println("RIGHT PRESSED"); }
  else if(digitalRead(startButton) == LOW) {pendingStart = true; Serial.println("START PRESSED"); }
}
