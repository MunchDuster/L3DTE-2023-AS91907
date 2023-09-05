//Deliverable 3
//Parts: L298N, LEDs (x3), Buttons (x5), Accelerometer, Buzzer

//Wiring:
//L298N - Arduino Nano//
//GND - GND
//+5V - Vin
//IN1 - A0
//IN2 - A1
//IN3 - A2
//IN4 - A3
//Piezo buzzer - Arduino//
//(-) - GND
//(+) - D2
//MPU6050 - Arduino Nano//
//VCC - 5V
//GND - GND
//SCL - A5
//SDA - A4
//Buttons - Arduino Nano//
//(- of all buttons) - GND
//(+ of left button) - D12
//(+ of right button) - D11
//(+ of foward button) - D10
//(+ of backward button) - D9
//(+ of go button) - D8

#include <List.hpp>
#include <MPU6050.h>

//Note: EMPTY needed otherwise using NULL, FORWARD = 0 but also NULL = 0
enum Instruction { FORWARD, BACKWARD, LEFT, RIGHT, EMPTY };

//Accelerometer (MPU6050) constants
const double minimumReading = -32768; //Lowest reading from sensor (2 byte signed number)
const double maximumReading = 32767; //Highest reading from sensor (2 byte signed number)
const double gyroRangeDegreesPerSecond = 250; //The highest reading from sensor in degrees per second
const int deltaTimeMilliseconds = 10; //Milliseconds between iterations
const double deltaTimeSeconds = (double)deltaTimeMilliseconds / 1000.0;
const double turnReading = 160.0; //Setting the targetOutput of a turn to this value results in a roughly 90 degree turn

//Button input pins
const int leftButton = 11;
const int goButton = 10;
const int rightButton = 12;
const int forwardButton = 8;
const int backButton = 9;

//LED output pins
const int runningLED = A0;
const int waitingLED = A1;
const int powerLED = A2;

//Motor output pins
const int IN1 = 3;
const int IN2 = 4;
const int IN3 = 5;
const int IN4 = 6;

//Piezo buzzer pin
const int buzzer = 2;

//Control settings
const int TURN_TIME = 900; //Should turn about 90 degrees
const int DRIVE_TIME_FORWARD = 600; //Should drive about 20 centimeters
const int DRIVE_TIME_BACKWARD = 700; //Should drive about 20 centimeters
const int BRAKE_TIME = 120; //Reverses motors for a short time to brake
const int READ_GAP = 5; //Milliseconds between each MPU6050 reading
const int REFRESH_RATE = 100; //FPS in Hz
const int DELTA_TIME = 1000 / REFRESH_RATE; //Milliseconds between frames
const int BUTTON_CHECK_DELAY = 20; //50 Hz refresh rate
const int FINISHED_GAP = 200;
const int INSTRUCTION_GAP = 200;
const int START_RUNNING_GAP = 1000;

//PID settings
const double proportionalFactor = 60;
const double integralFactor = 0;
const double derivativeFactor = 20;

//Piezo buzzer song data
//Song array order: 0 is number of notes in song, 1 is noteNumber, 2 is noteDuration, 3 is noteGap (repeat 1,2,3)
const int songs[5][17] = {
  /*Startup    */ {4, 0, 50, 60, 12, 50, 60, 12, 50, 60, 0, 100, 200},
  /*Input      */ {2, 0, 50, 50, 8, 50, 50},
  /*Running    */ {4, 5, 100, 100, 5, 100, 100, -12, 100, 100, -17, 400, 100},
  /*Finished1  */ {2, -12, 50, 50, -12, 50, 50},
  /*FinishedAll*/ {4, -20, 200, 100, -20, 200, 100, -12, 200, 100, -12, 400, 100},
};

//PID variables
double outputDerivative; //Sensor reads in degrees/second, must be integrated to get output
double output; // In degrees, the actual sensor output value
double targetOutput; // In degrees, the ideal output value
double errorDerivative; // In degrees per second
double error; // The difference between the output and targetOutput
double errorIntegral; // In degrees seconds
double pidValue; //The PID output value (combined error, errorDerivative and errorIntegral)

//Accelerometer variables
MPU6050 accelgyro; //variable to communicate with sensor
int16_t ax, ay, az, gx, gy, gz; //holds sensor raw output values

//Instruction list for holding inputs to execute
List<Instruction> instructions;

//Button input variables
Instruction pendingInstruction = EMPTY;
Instruction lastInstruction = EMPTY;
bool pendingStart;

//Motor state variables
int motorLeft;
int motorRight;

//Functions
void setup() {
  //Initialize serial output (console for debugging)
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
  pinMode(goButton, INPUT_PULLUP);

  //Initialize MPU6050
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU Connection Success" : "MPU Connection FAIL");

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
  digitalWrite(waitingLED, LOW);
  digitalWrite(runningLED, HIGH);
  //Play running song
  playSong(2);
  delay(START_RUNNING_GAP);
  RunInstructions();

  //Finished
  Serial.println("FINISHED RUNNING");
  //Play finished all instructions song
  playSong(4);
  delay(FINISHED_GAP);
}

///MAIN FUNCTIONS///
void GetInput() {
  Serial.println("GETTING INPUT");
  while (!pendingStart) {
    //Get input
    CheckButtons();
    //Add input to instructions
    if (pendingInstruction != EMPTY) {
      //Play input song if button pressed
      playSong(1);
      //Add to list
      instructions.add(pendingInstruction);
      //Update
      lastInstruction = pendingInstruction;
      pendingInstruction = EMPTY;
      Serial.println("ADDING INSTRUCTION");
    }

    delay(BUTTON_CHECK_DELAY);
  }
  pendingStart = false;
  pendingInstruction = EMPTY;
}
void RunInstructions() {
  for (int i = 0; i < instructions.getSize(); i++) {
    switch (instructions.getValue(i)) {
      case FORWARD: Drive(1); break;
      case BACKWARD: Drive(-1); break;
      case LEFT: Turn(1); break;
      case RIGHT: Turn(-1); break;
    }
    //Play finished instruction song
    playSong(3);
    Stop(INSTRUCTION_GAP);
  }
  
  instructions.clear();
}

///INPUT BUTTONS///
void CheckButtons() {
  //Wait for last button to be released
  if (lastInstruction != EMPTY) {
    int checkButton;
    switch (lastInstruction) {
      case FORWARD: checkButton = forwardButton; break;
      case BACKWARD: checkButton = backButton; break;
      case LEFT: checkButton = leftButton; break;
      case RIGHT: checkButton = rightButton; break;
    }
    //Check the button is not being pressed
    if (digitalRead(checkButton) == HIGH) {
      lastInstruction = EMPTY;
    }
    else {
      Serial.println("NOT UNPRESSED, SKIPPING");
      return;
    }
  }
  //Because buttons are pullup, LOW is when pressed
  if (digitalRead(forwardButton) == LOW) {  
    pendingInstruction = FORWARD;
    Serial.println("FORWARD PRESSED");
  }
  if (digitalRead(backButton) == LOW) {
    pendingInstruction = BACKWARD;
    Serial.println("BACKWARD PRESSED");
  }
  if (digitalRead(leftButton) == LOW) {
    pendingInstruction = LEFT;
    Serial.println("LEFT PRESSED");
  }
  if (digitalRead(rightButton) == LOW) {
    pendingInstruction = RIGHT;
    Serial.println("RIGHT PRESSED");
  }
    if (digitalRead(goButton) == LOW) {
      pendingStart = true;
      Serial.println("START PRESSED");
    }
}

///MOTOR CONTROL FUNCTIONS///
//Interprets instructions for each motor and sets inputs
void SetMotors(int inputLeft, int inputRight) {
  //Set the left motor inputs
  bool movingLeft = inputLeft != 0;
  bool moveDirectionLeft = inputLeft > 0;
  digitalWrite(IN1, movingLeft && moveDirectionLeft);
  digitalWrite(IN2, movingLeft && !moveDirectionLeft);

  //Set the right motor inputs
  bool movingRight = inputRight != 0;
  bool moveDirectionRight = inputRight > 0;
  digitalWrite(IN3, movingRight && moveDirectionRight);
  digitalWrite(IN4, movingRight && !moveDirectionRight);

  //Update vars
  motorLeft = inputLeft;
  motorRight = inputRight;
}
//Moves the car 20ish centimeters forward or backward
void Drive(int forward) {
  Serial.println(forward == 1 ? "Driving forward" : "Driving backward");
  SetMotors(-forward, -forward);
  delay(forward == 1 ? DRIVE_TIME_FORWARD : DRIVE_TIME_BACKWARD);
}
//Turns the car by 90ish degrees
void Turn(int left) {
  Serial.println(left == 1 ? "Turning left" : "Turning right");
  ResetPID();
  targetOutput = (double)left * turnReading; //Turn by 90 degrees

  for (int curMillis = 0; curMillis <= TURN_TIME; curMillis += DELTA_TIME) {
    UpdatePID();

    if (pidValue > 0)
      //Too far right, turn left
      SetMotors(-1, 1);
    else
      //Too far left, turn right
      SetMotors(1, -1);
  }
}
//Stops the car moving
void Stop(int waitTime) {
  SetMotors(-motorLeft, -motorRight);
  delay(BRAKE_TIME);
  SetMotors(0, 0);
  delay(waitTime - BRAKE_TIME);
}

///PIEZO BUZZER FUNCTIONS///
//Plays a song from the songs array at index
void playSong(int songIndex) {
  //Foreach note in the song
  int notesInSong = songs[songIndex][0];
  for (int noteNumber = 0; noteNumber < notesInSong; noteNumber++) {
    //Play the note
    int noteIndex = noteNumber * 3 + 1;
    playNote(songs[songIndex][noteIndex], songs[songIndex][noteIndex + 1], songs[songIndex][noteIndex + 2]);
  }
}
//Plays a note for duration nefore delaying by gap
void playNote(int noteNumber, int noteDuration, int noteGap) {
  double frequency = 440.0 * pow(pow(2.0, 1.0 / 12.0), noteNumber); // frequency of note in half steps from A4
  tone(buzzer, (int)frequency); //Play the sound to the nearest integer frequency (terrible for low frequencies but fine for above 100Hz)
  delay(noteDuration);
  noTone(buzzer); //Shoosh
  delay(noteGap);
}

///PID FUNCTIONS///
//Reads sensor values and calculates new PID variable values
void UpdatePID() {
  for (int curMillis = 0; curMillis < DELTA_TIME; curMillis += READ_GAP) {
    //Read MPU6050 values
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    //Scale value from output units to degrees per second
    outputDerivative = map((double)gz, minimumReading, maximumReading, -gyroRangeDegreesPerSecond, gyroRangeDegreesPerSecond);
    //Add to angle
    output += outputDerivative * deltaTimeSeconds;
    //Error calculations
    double lastError = error;
    error = targetOutput - output;
    errorDerivative = (error - lastError) / deltaTimeSeconds;
    errorIntegral += error * deltaTimeSeconds;
    //PID value calculation
    pidValue = error * proportionalFactor + errorDerivative * derivativeFactor + errorIntegral * integralFactor;
    delay(READ_GAP);
  }

  //Log to serial
  Serial.println(pidValue);
}
//Resets all PID variables
void ResetPID() {
  output = 0;
  outputDerivative = 0;
  errorDerivative = 0;
  errorIntegral = 0;
  pidValue = 0;
}
