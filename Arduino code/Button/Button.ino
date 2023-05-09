//Button test
//Tests 1 button, logs output to console

//Wiring (+ve and -ve of button are any pins on same side, button should be in center of breadboard)
//Button - Arduino Nano
//+ve - D9
//-ve - GND

#define buttonPin 9

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  bool buttonValue = digitalRead(buttonPin);
  Serial.println(buttonValue);
}
