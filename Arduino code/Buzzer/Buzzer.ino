// Piezo Speaker test : Plays a little song in A major
// Tutorial link: http://www.ardumotive.com/how-to-use-a-buzzer-en.html

//Note: Buzzer should have at least 100 ohm resistor from arduino into positive terminal
const int buzzerPin = 2;

//Song constants
const int noteGap = 30;
const int noteDuration = 100;
const int notes[] = {1, 5, 8, 12, 17, 12, 8, 5}; //Notes in bar
const int bars[] = {0, -5, 0, -7}; //Bars in song
  
void setup(){
  pinMode(buzzerPin, OUTPUT); // Set buzzerPin as an output
}

void loop(){
  for(int b = 0; b < 4; b++) {
    for(int n = 0; n < 8; n++) {
      playNote(notes[n] + bars[b]);
    }
  }
}

void playNote(int n) {
  double frequency = 440.0 * pow(pow(2.0, 1.0/12.0), n); // frequency of note in half steps from A4
  tone(buzzerPin, (int)frequency); //Play the sound to the nearest integer frequency
  delay(noteDuration);
  noTone(buzzerPin); //Quiet
  delay(noteGap);
}
