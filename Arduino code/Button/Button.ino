//Button test
//Tests 1 button, logs output to console

//Wiring (+ve and -ve of button are any pins on same side, button should be in center of breadboard)
//Button - Arduino Nano
//+ve - D9
//-ve - GND

#define buttonPin 8

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  bool buttonDown = !digitalRead(buttonPin); //Pull up is reverse normal input
  digitalWrite(LED_BUILTIN, buttonDown);
}
