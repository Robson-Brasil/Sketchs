/********************************************************
 * CANAL INTERNET E COISAS
 * OTA - HTTP Update
 * Multiplataforma ESP32 e ESP8266
 * 03/2020 - Andre Michelon
 * andremichelon@internetecoisas.com.br
 * https://internetecoisas.com.br
 * Historico
 * 02/04/2021 Revisao para compatibilidade de WiFiClientSecure
 */

// Bibliotecas ------------------------------------------
#ifdef ESP8266
  // Bibliotecas para ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266httpUpdate.h>
  #include <FS.h>
#else
  // Bibliotecas para ESP32
  #include <WiFi.h>
  #include <HTTPUpdate.h>
  #include <SPIFFS.h>
#endif

// Constantes -------------------------------------------
// Wi-Fi
const char*   SSID      = "home2.4";
const char*   PASSWORD  = "Home31415";

// Setup ------------------------------------------------
void setup() {
  #ifdef ESP8266
    // Inicialização para ESP8266
    Serial.begin(74880);
  #else
    // Inicialização para ESP32
    Serial.begin(115200);
  #endif
  Serial.println("\nIniciando");

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
    Serial.println("Arquivo não encontrado");
  }

  // Conecta WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.println("\nConectando WiFi " + String(SSID));
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

#ifdef ESP8266
  // Definições para ESP8266

  // LED indicador de progresso
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

  // Callback - Início
  ESPhttpUpdate.onStart([]{
    Serial.println("Atualização iniciada (callback)");
  });

  // Callback - Fim
  ESPhttpUpdate.onEnd([]{
    Serial.println("\nAtualização concluída (callback)");
  });

  // Callback - Erro
  ESPhttpUpdate.onError([](int erro) {
    Serial.println("\nErro na atualização (callback), código: " + String(erro));
  });

  // Callback - Progresso
  ESPhttpUpdate.onProgress([](size_t progresso, size_t total) {
    Serial.print(progresso * 100 / total);
    Serial.print(" ");
  });

  // Não reiniciar automaticamente
  ESPhttpUpdate.rebootOnUpdate(false);
#else
  // Definições para ESP32

  // LED indicador de progresso
  httpUpdate.setLedPin(2, HIGH);

  // Callback - Progresso
  Update.onProgress([](size_t progresso, size_t total) {
    Serial.print(progresso * 100 / total);
    Serial.print(" ");
  });

  // Não reiniciar automaticamente
  httpUpdate.rebootOnUpdate(false);
#endif

  // Cria instância de Cliente seguro
  WiFiClientSecure client;

  // Instrui Cliente a ignorar assinatura do Servidor na conexao segura
  client.setInsecure();

  // Atualização SPIFFS ---------------------------------
  Serial.println("\n*** Atualização da SPIFFS ***");
  for (byte b = 5; b > 0; b--) {
    Serial.print(b);
    Serial.println("... ");
    delay(1000);
  }
  // Encerra SPIFFS
  SPIFFS.end();

  // Efetua atualização da SPIFFS
#ifdef ESP8266
  // ESP8266
  t_httpUpdate_return resultado = ESPhttpUpdate.updateSpiffs(client, "https://internetecoisas.com.br/download/IeC115-OTA/Blink.spiffs.nodemcu.bin");
#else
  // ESP32
  t_httpUpdate_return resultado = httpUpdate.updateSpiffs(client, "https://internetecoisas.com.br/download/IeC115-OTA/Blink.spiffs.esp32.bin");
#endif

  // Verifica resultado
  switch (resultado) {
    case HTTP_UPDATE_FAILED: {
      #ifdef ESP8266
        // ESP8266
        String s = ESPhttpUpdate.getLastErrorString();
      #else
        // ESP32
        String s = httpUpdate.getLastErrorString();
      #endif
      Serial.println("\nFalha: " + s);
    } break;
    case HTTP_UPDATE_NO_UPDATES: {
      Serial.println("\nNenhuma atualização disponível");
    } break;
    case HTTP_UPDATE_OK: {
      Serial.println("\nSucesso");
    } break;
  }

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
    Serial.println("Arquivo não encontrado");
  }

  // Atualização Sketch ---------------------------------
  Serial.println("\n*** Atualização do sketch ***");
  for (byte b = 5; b > 0; b--) {
    Serial.print(b);
    Serial.println("... ");
    delay(1000);
  }

  // Efetua atualização do Sketch
#ifdef ESP8266
  // ESP8266
  resultado = ESPhttpUpdate.update(client, "https://internetecoisas.com.br/download/IeC115-OTA/Blink.ino.nodemcu.bin");
#else
  // ESP32
  resultado = httpUpdate.update(client, "https://internetecoisas.com.br/download/IeC115-OTA/Blink.ino.esp32.bin");
#endif

  // Verifica resultado
  switch (resultado) {
    case HTTP_UPDATE_FAILED: {
      #ifdef ESP8266
        // ESP8266
        String s = ESPhttpUpdate.getLastErrorString();
      #else
        // ESP32
        String s = httpUpdate.getLastErrorString();
      #endif
      Serial.println("\nFalha: " + s);
    } break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("\nNenhuma atualização disponível");
      break;
    case HTTP_UPDATE_OK: {
      Serial.println("\nSucesso, reiniciando...");
      ESP.restart();
    } break;
  }
}

// Loop --------------------------------------------
void loop() {
  // Nada aqui
}
