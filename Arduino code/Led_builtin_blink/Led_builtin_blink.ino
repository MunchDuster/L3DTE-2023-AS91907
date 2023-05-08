#define waitOff 300
#define shortBlink 300
#define longBlink 900
#define endOfSentenceWait 1500
#define pin 9

void setup() {
  pinMode(pin, OUTPUT);
}

void loop() {
  //S
  Blink(longBlink);
  Blink(longBlink);
  Blink(longBlink);
  //O
  Blink(shortBlink);
  Blink(shortBlink);
  Blink(shortBlink);
  //S
  Blink(longBlink);
  Blink(longBlink);
  Blink(longBlink);
  //Wait
  delay(endOfSentenceWait);
}

void Blink(int blinkTime)
{
  digitalWrite(pin, HIGH);
  delay(blinkTime);
  digitalWrite(pin, LOW);
  delay(waitOff);
}
