/*
Sensor BMP180 - Pressão, Altitude e Temperatura
*/

#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(115200);
  if (!bmp.begin()) {
	Serial.println("Não foi possível encontrar um sensor BMP085/BMP180 válido, verifique a fiação!");
	while (1) {}
  }
}
  
void loop() {
  Serial.print("Temperatura = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" ºC");
    
  Serial.print("Pressão = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
    
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" metros");

  Serial.print("Pressão ao nível do mar (calculada) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  Serial.print("Altitude Real = ");
  Serial.print(bmp.readAltitude(102000));
  Serial.println(" metros");
    
  Serial.println();
  delay(500);
}