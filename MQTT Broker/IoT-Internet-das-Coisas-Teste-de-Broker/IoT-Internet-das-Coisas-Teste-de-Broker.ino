//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT, Temperatura, Umidade e Relés
//Autor : Robson Brasil
//Versão : 55
//Última Modificação : 18/08/2021
//Obs.: Os Relés não estão funcionando no protocolo MQTT

#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
 
#define INTERVALO_ENVIO       1000
 
#define DHTPIN 16      // o sensor DHT22 foi conectado ao pino 2( D4 do node MCU)
#define DHTTYPE DHT22
 
DHT dht(DHTPIN, DHTTYPE);
 
#define DEBUG
 
#define Rele1 23   //pino de saida para acionamento do Rele1
#define Rele2 22   //pino de saida para acionamento do Rele2
#define Rele3 21  //D21
#define Rele4 19  //D19
#define Rele5 18  //D18
#define Rele6  5  //D5
#define Rele7 25  //D25
#define Rele8 26  //D26
 
//informações da rede WIFI
const char* ssid = "RVR 2,4GHz";                 //SSID da rede WIFI
const char* password =  "RodrigoValRobson2021";    //senha da rede wifi
 
//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "dashboardiot.com.br";   //servermqtt
const char* mqttUser = "emqx_test";              //user servermqtt
const char* mqttPassword = "emqx_test";      //password servermqtt
const int mqttPort = 1883;                     //port

//tópicos que serão assinados
#define mqttTopicSub  "casa/sala/rele1"        
#define mqttTopicSub  "casa/quarto/robson/rele2"
 
int ultimoEnvioMQTT = 0;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {

  Serial.begin(115200);
  pinMode(Rele1, OUTPUT);
  digitalWrite(Rele1,HIGH);
  pinMode(Rele2, OUTPUT);
  digitalWrite(Rele2,HIGH);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.println("Conectando ao WiFi..");
    #endif
  }
  #ifdef DEBUG
  Serial.println("Conectado na rede WiFi");
  #endif
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqtt_callback);
 
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
 
  if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      #ifdef DEBUG
      Serial.println("Conectado");  
      #endif
     } 
  else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000);
     }
  }
  //subscreve no tópico
  client.subscribe(mqttTopicSub);
  dht.begin();
 }
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  //armazena msg recebida em uma string
  payload[length] = '\0';
  String Rele1MQTT = String((char*)payload);

  //aciona saída conforme msg recebida 
  if (Rele1MQTT == "1"){         //se msg "1"
     digitalWrite(Rele1, LOW);
  }else if (Rele1MQTT == "0"){   //se msg "0"
     digitalWrite(Rele1, HIGH);
     
//     sprintf(Rele1MQTT.c_str(),"",Rele1);
     client.publish("casa/sala/rele1",Rele1MQTT.c_str());
  }
    String Rele2MQTT = String((char*)payload);
     //aciona saída conforme msg recebida 
  if (Rele2MQTT == "1"){         //se msg "1"
     digitalWrite(Rele2, LOW);//coloca saída em LOW para ligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
  }else if (Rele2MQTT == "0"){   //se msg "0"
     digitalWrite(Rele2, HIGH);//coloca saída em HIGH para desligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo

//      sprintf(Rele2MQTT,"",Rele2);
      client.publish("casa/quarto/robson/rele2",Rele2MQTT.c_str());
   }
  
  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(Rele1MQTT);
  Serial.print(Rele2MQTT);
  Serial.println();
  Serial.println("-----------------------");
  #endif
}
//função pra reconectar ao servido MQTT
void reconnect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
    #endif
     
    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("ESP32Client", mqttUser, mqttPassword) :
                     client.connect("ESP32Client");
 
    if(conectado) {
      #ifdef DEBUG
      Serial.println("Conectado!");
      #endif
      //subscreve no tópico
      client.subscribe(mqttTopicSub, 2); //nivel de qualidade: QoS 2
    } else {
      #ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
      #endif
      //Aguarda 10 segundos 
      delay(10000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
 
  //envia a cada X segundos
  if ((millis() - ultimoEnvioMQTT) > INTERVALO_ENVIO)
  {
      enviaDHT();
      ultimoEnvioMQTT = millis();
  }
  
  client.loop();
}
//função para leitura do DHT22
void enviaDHT(){
 
  char MsgUmidadeMQTT[10];
  char MsgTemperaturaMQTT[10];
  
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  
  if (isnan(temperatura) || isnan(umidade)) 
  {
    #ifdef DEBUG
    Serial.println("Falha na leitura do DHT22...");
    #endif
  } 
  else 
  {
    #ifdef DEBUG
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.print(" \n"); //quebra de linha
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
    #endif
 
    sprintf(MsgUmidadeMQTT,"%f",umidade);
    client.publish("casa/umidade", MsgUmidadeMQTT);
    sprintf(MsgTemperaturaMQTT,"%f",temperatura);
    client.publish("casa/temperatura", MsgTemperaturaMQTT);
  }
}
