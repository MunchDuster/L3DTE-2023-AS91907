//Perfboard tester
//Sets all the pins on the arduino as outputs and sets high voltage

const int pins[] = {2,3,4,5,6,7,8,9,10,11,12,LED_BUILTIN,A0,A1,A2,A3,A4,A5,A6,A7};
const int pinsCount = 18;

void setup() {
  for(int i = 0; i < pinsCount; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  //flash the pins
  for(int i = 0; i < pinsCount; i++) {
    digitalWrite(pins[i], HIGH);
    delay(50);
    digitalWrite(pins[i], LOW);
  }
}
