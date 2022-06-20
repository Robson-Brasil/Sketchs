// Automatic And Manual Control Home Automation using ESP32 and Blynk

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg";

// Your WiFi credentials.
char ssid[] = "RVR 2,4GHz";     // Your Network SSID
char pass[] = "RodrigoValRobson2021";     // Your Network PASSWARD

int RELAY[8] = {13, 12, 14, 27, 26, 25, 1, 3};          // Define Output pins connected to Relay
int SWITCH[8] = {23, 22, 21, 19, 18, 5, 33, 32};       // Define Input pins connected to Switch
int VPIN[8] = {V1, V2, V3, V4, V5, V6, V7, V8};     // Define Virtual pins assigned in the APP
//[note: you need to invese the Output logic (1 _- 0) and change it to Switch Mode]

bool RELAY_STATE[8] = {1, 1, 1, 1, 1, 1, 1, 1};
bool SWITCH_STATE[8] = {1, 1, 1, 1, 1, 1, 1, 1};

int STATUS_LED = 2;     //Shows Network Status

BLYNK_WRITE(V1) {
  RELAY_STATE[0] = param.asInt();
  digitalWrite(RELAY[0], RELAY_STATE[0]);
}
BLYNK_WRITE(V2) {
  RELAY_STATE[1] = param.asInt();
  digitalWrite(RELAY[1], RELAY_STATE[1]);
}
BLYNK_WRITE(V3) {
  RELAY_STATE[2] = param.asInt();
  digitalWrite(RELAY[2], RELAY_STATE[2]);
}
BLYNK_WRITE(V4) {
  RELAY_STATE[3] = param.asInt();
  digitalWrite(RELAY[3], RELAY_STATE[3]);
}
BLYNK_WRITE(V5) {
  RELAY_STATE[4] = param.asInt();
  digitalWrite(RELAY[4], RELAY_STATE[4]);
}
BLYNK_WRITE(V6) {
  RELAY_STATE[5] = param.asInt();
  digitalWrite(RELAY[5], RELAY_STATE[5]);
}
BLYNK_WRITE(V7) {
  RELAY_STATE[6] = param.asInt();
  digitalWrite(RELAY[6], RELAY_STATE[6]);
}
BLYNK_WRITE(V8) {
  RELAY_STATE[7] = param.asInt();
  digitalWrite(RELAY[7], RELAY_STATE[7]);
}

//this function will synchronize Relay States when any network/Hardware interruption occurs
BLYNK_CONNECTED() {
  //Uncomment this line if you want to follow app configurations
  //Blynk.syncAll();

  //Uncomment this line if you want to follow Hardware configurations
  for (int i = 0; i < 8; i++) {
    Blynk.virtualWrite(VPIN[i], RELAY_STATE[i]);
  }
}

// Checks Network connection status in every 5 seconds
void Network_Checkup() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(10);
  }
}

void ONLINE() {
  if (Blynk.connected()) {
    for (int i = 0; i < 8; i++) {
      if (digitalRead(SWITCH[i]) == LOW) {
        if (SWITCH_STATE[i] != LOW) {
          RELAY_STATE[i] = !RELAY_STATE[i];
          digitalWrite(RELAY[i], RELAY_STATE[i]);
          Blynk.virtualWrite(VPIN[i], RELAY_STATE[i]);
        }
        SWITCH_STATE[i] = LOW;
      }
      else {
        SWITCH_STATE[i] = HIGH;
      }
    }
  }
}

void OFFLINE() {
  for (int i = 0; i < 8; i++) {
    if (digitalRead(SWITCH[i]) == LOW) {
      if (SWITCH_STATE[i] != LOW) {
        RELAY_STATE[i] = !RELAY_STATE[i];
        digitalWrite(RELAY[i], RELAY_STATE[i]);
      }
      SWITCH_STATE[i] = LOW;
    }
    else {
      SWITCH_STATE[i] = HIGH;
    }
  }
}

void setup()
{
  pinMode(STATUS_LED, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(SWITCH[i], INPUT);
    pinMode(RELAY[i], OUTPUT);
    digitalWrite(RELAY[i], HIGH);
  }

  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  timer.setInterval(5000L, Network_Checkup);  // timer enable function
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(STATUS_LED, HIGH);
    Blynk.run();
    ONLINE();
  }
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(STATUS_LED, LOW);
    OFFLINE();
  }
  timer.run();
}
