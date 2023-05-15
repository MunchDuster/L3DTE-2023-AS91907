
// Rotary encoder test
// Logs encoder output to serial

// Wiring:
// Rotary Encoder - Arduino Nano
// GND - GND
// + (VCC) - 5V
// CLK (Out 1) - D2
//DT (Out 2) - D3
//SW (Button) - D4

#define rotaryInPin1 2
#define rotaryInPin2 3
#define rotaryButtonPin 4

bool rotaryIn1Last;
int count = 0;

void setup() {
  //Setup pins and serial
  pinMode(rotaryInPin1, INPUT);
  pinMode(rotaryInPin2, INPUT);
  pinMode(rotaryButtonPin, INPUT_PULLUP);
  Serial.begin(9600);

  //Init last state
  rotaryIn1Last = digitalRead(rotaryInPin1);
}

void loop() {
  bool rotaryIn1 = digitalRead(rotaryInPin1);

  //If state changed, increment or decrement count
  if(rotaryIn1 != rotaryIn1Last)
  {
    bool rotaryIn2 = digitalRead(rotaryInPin2);
    bool statesDifferent = rotaryIn1 != rotaryIn2;
    count += statesDifferent ? -1 : 1;
  }

  //Update last
  rotaryIn1Last = rotaryIn1;

  //Get button value
  bool buttonDown = !digitalRead(rotaryButtonPin); //Pull up is reverse

  //Print to console and wait a short amount of time
  Serial.print(count);
  Serial.print(" ");
  Serial.println(buttonDown);
}
