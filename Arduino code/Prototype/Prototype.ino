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

#include <LiquidCrystal_I2C.h>
#include <MPU6050.h>

//Note: EMPTY needed otherwise using NULL, FORWARD = 0
enum Instruction { FORWARD, BACKWARD, LEFT, RIGHT };

//Accelerometer (MPU6050) constants
const double minimumReading = -32768; //Lowest reading from sensor (2 byte signed number)
const double maximumReading = 32767; //Highest reading from sensor (2 byte signed number)
const double gyroRangeDegreesPerSecond = 250; //The highest reading from sensor in degrees per second
const int deltaTimeMilliseconds = 10; //Milliseconds between iterations
const double deltaTimeSeconds = (double)deltaTimeMilliseconds / 1000.0;

//Button input pins
const int joystickButton = 10;
const int joystickAxisY = 11;
const int joystickAxisX = 12;

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);  

//Motor output pins
const int IN1 = 3;
const int IN2 = 4;
const int IN3 = 5;
const int IN4 = 6;

//Piezo buzzer pin
const int buzzer = 2;

//Control settings
const int TURN_TIME = 1500; //Should turn about 90 degrees
const int DRIVE_TIME_FORWARD = 600; //Should drive about 20 centimeters
const int DRIVE_TIME_BACKWARD = 700; //Should drive about 20 centimeters
const int BRAKE_TIME = 120; //Reverses motors for a short time to brake
const int READ_GAP = 5; //Milliseconds between each MPU6050 reading
const int REFRESH_RATE = 100; //FPS in Hz
const int DELTA_TIME = 1000 / REFRESH_RATE; //Milliseconds between frames
const int BUTTON_CHECK_DELAY = 20; //50 Hz refresh rate

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
double outputDerivative; // sensor reads in degrees/second, must be integrated to get output
double output; // in degrees, the actual sensor output value
double targetOutput; // in degrees, the ideal output value
double errorDerivative; // in degrees per second
double error; // the difference between the output and targetOutput
double errorIntegral; // in degrees seconds
double pidValue; // the PID output value (combined error, errorDerivative and errorIntegral)

//Accelerometer variables
MPU6050 accelgyro; // variable to communicate with sensor
int16_t ax, ay, az, gx, gy, gz; // holds sensor raw output values


byte throttle, steer; // raw instructions from controller
double throttle, steer; // processed instructions from controller

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

  lcd.init();

  //Set buttons as input pullups
  pinMode(joystickButton, INPUT);
  pinMode(joystickAxisX, INPUT);
  pinMode(joystickAxisY, INPUT);

  //Initialize MPU6050
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU Connection Success" : "MPU Connection FAIL");

  //Turn power LED on
  digitalWrite(powerLED, HIGH);

  //Play starting song
  playSong(0);
}

void loop() {
  GetInput();
  UpdateStatus("Idle", "X:" + GetDisplayFloat(valueX) + " Y:" + GetDisplayFloat(valueY));
  delay(START_RUNNING_GAP);
  RunInstructions();
  delay(FINISHED_GAP);
}

///MAIN FUNCTIONS///
void GetInput() {
  // Use wireless device to scan for instructions
  // Record input throttle and steer

}
void RunInstructions() {
  instructionIndex = 0;
  for (int i = 0; i < instructionCount; i++) {
    switch (instructions[i]) {
      case FORWARD: Drive(1); break;
      case BACKWARD: Drive(-1); break;
      case LEFT: Turn(1); break;
      case RIGHT: Turn(-1); break;
    }
    //Play finished instruction song
    playSong(3);
    Stop(INSTRUCTION_GAP);
  }
}

///INPUT BUTTONS///
void ReadButtons() {
  bool isForwardPressed = !digitalRead(forwardButton);
  bool isBackPressed = !digitalRead(backButton);
  bool isRightPressed = !digitalRead(rightButton);
  bool isLeftPressed = !digitalRead(leftButton);
  bool isGoPressed = !digitalRead(goButton);

  if (isForwardPressed) {
    pendingInstruction = FORWARD;
    Serial.println("FORWARD PRESSED");
  }
  if (isBackPressed) {
    pendingInstruction = BACKWARD;
    Serial.println("BACKWARD PRESSED");
  }
  if (isRightPressed) {
    pendingInstruction = LEFT;
    Serial.println("LEFT PRESSED");
  }
  if (isLeftPressed) {
    pendingInstruction = RIGHT;
    Serial.println("RIGHT PRESSED");
  }
  if (isGoPressed) {
    isPendingStart = true;
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

///SCREEN FUNCTIONS///
void UpdateStatus(String status, String secondLine)
{
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Status: " + status);
  lcd.setCursor(0,1);
  lcd.print(secondLine);
}

String GetDisplayFloat(float num)
{
  String numStr = String(num, 1); //Number to string with 2 decimal places
  if(numStr.length() == 3) return " " + numStr;
  else return numStr;
}