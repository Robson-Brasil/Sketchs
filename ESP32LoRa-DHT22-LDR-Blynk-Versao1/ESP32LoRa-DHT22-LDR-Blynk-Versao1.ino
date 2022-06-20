#include "DHTesp.h"
#include "heltec.h"
DHTesp dht;
 
float currentTemp;
float currentHumidity;
 
void displayReadingsOnOled() {
   
  String temperatureDisplay ="Temperatura: " + (String)currentTemp +  "°C";
  String humidityDisplay = "Humidade: "      + (String)currentHumidity +   "%";
 
  // Clear the OLED screen
  Heltec.display->clear();
  // Prepare to display temperature
  Heltec.display->drawString(0, 0, temperatureDisplay);
  // Prepare to display humidity
  Heltec.display->drawString(0, 12, humidityDisplay);
  // Display the readings
  Heltec.display->display();
}
    
void setup()
{
 
  dht.setup(22, DHTesp::DHT22);
   
  currentTemp = dht.getTemperature();
  currentHumidity = dht.getHumidity();
 
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
 
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, false /*Serial Enable*/);
  displayReadingsOnOled();
}

void loop()
{
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
 
  if (temperature != currentTemp || humidity != currentHumidity) {
    currentTemp = temperature;
    currentHumidity = humidity;
    displayReadingsOnOled();
  }
    
  delay(2000);
}
