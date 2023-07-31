// Piezo Speaker test
// beeps buzzer at 440Hz (A4 note)
// Tutorial link: http://www.ardumotive.com/how-to-use-a-buzzer-en.html

const int buzzer = 9; //buzzer to arduino pin 9

void setup(){
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
}

void loop(){
  tone(buzzer, 440); // Send sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
}
