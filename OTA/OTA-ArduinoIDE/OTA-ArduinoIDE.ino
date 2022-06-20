/********************************************************
 * CANAL INTERNET E COISAS
 * OTA - Arduino IDE
 * Multiplataforma ESP32 e ESP8266
 * 03/2020 - Andre Michelon
 * andremichelon@internetecoisas.com.br
 * https://internetecoisas.com.br
 */

// Bibliotecas ------------------------------------------
#ifdef ESP8266
  // Bibliotecas para ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <FS.h>
#else
  // Bibliotecas para ESP32
  #include <WiFi.h>
  #include <ESPmDNS.h>
  #include <SPIFFS.h>
#endif

// Bibliotecas comuns
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Constantes -------------------------------------------
// Wi-Fi
const char*   SSID      = "RVR 2,4GHz";
const char*   PASSWORD  = "RodrigoValRobson2021";

// Setup ------------------------------------------------
void setup() {
  #ifdef ESP8266
    // Velocidade para ESP8266
    Serial.begin(74880);
  #else
    // Velocidade para ESP32
    Serial.begin(115200);
  #endif
  Serial.println("\nIniciando.");

  // Inicializa SPIFFS
  if (SPIFFS.begin()) {
    Serial.println("SPIFFS Ok");
  } else {
    Serial.println("SPIFFS Falha");
  }

  // Verifica / exibe arquivo
  if (SPIFFS.exists("/Teste.txt")) {
    File f = SPIFFS.open("/Teste.txt", "r");
    if (f) {
      Serial.println("Lendo arquivo:");
      Serial.println(f.readString());
      f.close();
    }
  } else {
    Serial.println("Arquivo não encontrado.");
  }

  // Conecta WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.println("\nConectando WiFi " + String(SSID));
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  /* Permite definir porta para conexão
      Padrão: ESP8266 - 8266
              ESP32   - 3232              */
  //ArduinoOTA.setPort(port);

  /* Permite definir nome do host
      Padrão: ESP8266 - esp8266-[ChipID]
              ESP32   - esp32-[MAC]       */
   ArduinoOTA.setHostname("ESP32-LoRa");

  /* Permite definir senha para acesso
      Padrão: sem senha                   */
   //ArduinoOTA.setPassword("@Lobo#Alfa@");

  /* Permite definir senha para acesso via Hash MD5
      Padrão: sem senha                   */
  // ArduinoOTA.setPasswordHash("senhaHashMD5");

  // Define funções de callback do processo
  // Início
  ArduinoOTA.onStart([](){
    String s;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      // Atualizar sketch
      s = "Sketch";
    } else { // U_SPIFFS
      // Atualizar SPIFFS
      s = "SPIFFS";
      // SPIFFS deve ser finalizada
      SPIFFS.end();
    }
    Serial.println("Iniciando OTA - " + s);
  });

  // Fim
  ArduinoOTA.onEnd([](){
    Serial.println("\nOTA Concluído.");
  });

  // Progresso
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.print(progress * 100 / total);
    Serial.print(" ");
  });

  // Falha
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.print("Erro " + String(error) + " ");
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Falha de autorização");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Falha de inicialização");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Falha de conexão");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Falha de recebimento");
    } else if (error == OTA_END_ERROR) {
      Serial.println("Falha de finalização");
    } else {
      Serial.println("Falha desconhecida");
    }
  });

  // Inicializa OTA
  ArduinoOTA.begin();

  // Pronto
  Serial.println("Atualização via OTA disponível.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Loop --------------------------------------------
void loop() {
  ArduinoOTA.handle();
}
