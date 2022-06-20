/********************************************************
 * CANAL INTERNET E COISAS
 * ESP32 Hello World
 * 07/2019 - Andre Michelon
 */

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World");
}

void loop() {
  Serial.println(millis() / 1000);
  delay(1000);
}
