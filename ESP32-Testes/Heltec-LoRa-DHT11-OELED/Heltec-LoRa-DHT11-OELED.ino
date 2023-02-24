#include "DHTesp.h"
#include "heltec.h"
DHTesp dht;
 
float currentTemp;
float currentHumidity;

TaskHandle_t Task1;
TaskHandle_t Task2;

const int led1 = 25;
 
void displayReadingsOnOled() {
   
  String temperatureDisplay ="Temperatura: " + (String)currentTemp +  "°C";
  String humidityDisplay = "Umidade: " + (String)currentHumidity + "%";
 
  // Clear the OLED screen
  Heltec.display->clear();
  // Prepare to display temperature
  Heltec.display->drawString(0, 0, temperatureDisplay);
  // Prepare to display humidity
  Heltec.display->drawString(0, 12, humidityDisplay);
  // Display the readings
  Heltec.display->display();
}
    
void setup(){

  Serial.begin(115200);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());

  for(;;){
    dht.setup(22, DHTesp::DHT11);
   
    currentTemp = dht.getTemperature();
    currentHumidity = dht.getHumidity();

    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
 
  if (temperature != currentTemp || humidity != currentHumidity) {
    currentTemp = temperature;
    currentHumidity = humidity;
    displayReadingsOnOled();
  }

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, false /*Serial Enable*/);
  displayReadingsOnOled();
  
  delay(2000);  

  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(led1, HIGH);
    delay(700);
    digitalWrite(led1, LOW);
    delay(700);
  }
}

void loop()
{

}