//HC-SR04 Ultra sonic sensor test

//Wiring:
//Ultrasonic - Arduino Nano
//Vcc - 5V
//Trig - D3
//Echo - D4
//Gnd - GND

#define trigPin 3
#define echoPin 4
#define speedOfSoundMicrosecondsPerCentimeter 29 
#define pulseTimeoutMilliseconds 10000

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //Create pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Measure time and calculate distance
  duration = pulseIn(echoPin, HIGH, pulseTimeoutMilliseconds);
  distance = duration / speedOfSoundMicrosecondsPerCentimeter / 2;
  
  Serial.println(distance);
}
