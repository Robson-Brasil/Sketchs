#include <WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>
#include <SPI.h>      // RC522 Module uses SPI protocol
#include <Wire.h>


// Definir o nome e senha da rede WiFi
const char* ssid = "RVR 2,4GHz";
const char* password = "RodrigoValRobson2021";

// Definir as informações do servidor MQTT
const char* mqttServer = "192.168.15.10";
const int mqttPort = 1883;
const char* mqttUser = "RobsonBrasil";
const char* mqttPassword = "loboalfa";

// Definir os tópicos MQTT para publicação e inscrição
const char* subtopic = "TOPICO-DE-LEITURA-DE-CARTAO";
const char* pubtopicautorizado = "TOPICO-DE-ACESSO-AUTORIZADO";
const char* pubtopicnegado = "TOPICO-DE-ACESSO-NEGADO";

// Definir os pinos usados pelo leitor de RFID
#define RST_PIN         2          // pino RST
#define SS_PIN          5         // pino SDA

// Inicializar o cliente WiFi
WiFiClient wifiClient;

// Inicializar o cliente MQTT
PubSubClient client(mqttServer, mqttPort, wifiClient);

// Inicializar o leitor de RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);

  // Conectar à rede WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede WiFi...");
  }
  Serial.println("Conectado à rede WiFi");

  // Conectar ao servidor MQTT
  client.setCallback(callback);
  client.setServer(mqttServer, mqttPort);
  
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.print("Falha na conexão com o servidor MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }

  // Configurar o leitor de RFID
  mfrc522.PCD_Init();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");

  String data = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  if (strcmp(topic, subtopic)) {
    for (unsigned int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      data += (char)payload[i];
    }
  }    

}
  
void reconnect() {
  // ...
}
void loop() {
  // Verificar se há novos cartões presentes
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Obter o UID do cartão em hexadecimal
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    // Publicar o UID do cartão no tópico correspondente
    if (client.publish(subtopic, uid.c_str())) {
      Serial.println("UID do cartão enviado com sucesso");
    } else {
      Serial.println("Falha ao enviar o UID do cartão");
    }
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(10);  // Aguardar um pouco entre cada leitura do cartão
}
