/********************
 *  Program:  HC-SR04 sensor tester
 *  Description: print distance to serial
 ********************/

#define TRIG_PIN 22
#define ECHO_PIN 23

void setup(){
  Serial.begin(115200); // initialize the serial port
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
}

// Define a new function that reads and converts the raw reading to distance (cm)
float distance_centimetre() {
  // Send sound pulse
  digitalWrite(TRIG_PIN, HIGH); // pulse started
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN, LOW); // pulse stopped

  // listen for echo 
  float tUs = pulseIn(ECHO_PIN, HIGH); // microseconds
  float distance = tUs / 58; // cm 
  return distance;
}

void loop(){
  Serial.print(distance_centimetre(), DEC);
  Serial.println("cm");
  delay(1000); // ms 
}