/********************************************************
 * CANAL INTERNET E COISAS
 * Blink
 * Multiplataforma ESP32 e ESP8266
 * 03/2020 - Andre Michelon
 * andremichelon@internetecoisas.com.br
 * https://internetecoisas.com.br
 */

// Definições -------------------------------------------
#ifdef ESP8266
  // Plataforma
  String s = "ESP8266";

  // ESP8266 possui pino padrão de LED
  const byte      LED_PIN                 = LED_BUILTIN;

  // ESP8266 utiliza o estado inverso
  const byte      LED_ON                  = LOW;
  const byte      LED_OFF                 = HIGH;
#else
  // Plataforma
  String s = "ESP32";

  // ESP32 não possui pino padrão de LED
  const byte      LED_PIN                 = 2;

  // ESP32 utilizam estado normal
  const byte      LED_ON                  = HIGH;
  const byte      LED_OFF                 = LOW;
#endif

// Setup ------------------------------------------------
void setup() {
  #ifdef ESP8266
    // Velocidade para ESP8266
    Serial.begin(74880);
  #else
    // Velocidade para ESP32
    Serial.begin(115200);
  #endif

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  Serial.println("\n\nIniciando blink - " + s);
}

// Loop --------------------------------------------
void loop() {
  digitalWrite(LED_PIN, LED_ON);
  delay(300);
  digitalWrite(LED_PIN, LED_OFF);
  delay(300);
}
