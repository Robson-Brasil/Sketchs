//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT, Temperatura, Umidade e Relés
//Autor : Robson Brasil
//Versão : 18
//Última Modificação : 17/08/2021
//Obs.: Os Relés não estão funcionando no protocolo MQTT

#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
 
#define INTERVALO_ENVIO       1000

/* Definições do DHT22*/
#define DHTPIN  16  //Sensor DHT22

// o sensor DHT22 foi conectado ao pino 16
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

/* Definições do BOTÃO */
#define Botao1  13  //D13
#define Botao2  12  //D12
#define Botao3  14  //D14
#define Botao4  27  //D27
#define Botao5  33  //D33
#define Botao6  32  //D32
#define Botao7  15  //D15
#define Botao8   4  //D4

/* Definições do LDR*/
#define LDR     36  //Sensor LDR

//#define MSGUmidadeMQTT umidade
//#define MSGTemperaturaMQTT temperatura
//#define MSGLuminosidadeMQTT luminosidade

//WiFi Status LED
#define wifiLed    0   //D0

int EstadoDoBotao1 = 1; //Define integer to remember the toggle state for relay 1
int EstadoDoBotao2 = 1; //Define integer to remember the toggle state for relay 2
int EstadoDoBotao3 = 1; //Define integer to remember the toggle state for relay 3
int EstadoDoBotao4 = 1; //Define integer to remember the toggle state for relay 4
int EstadoDoBotao5 = 1; //Define integer to remember the toggle state for relay 1
int EstadoDoBotao6 = 1; //Define integer to remember the toggle state for relay 2
int EstadoDoBotao7 = 1; //Define integer to remember the toggle state for relay 3
int EstadoDoBotao8 = 1; //Define integer to remember the toggle state for relay 4

 
//informações da rede WIFI
const char* ssid = "RVR 2,4GHz";                 //SSID da rede WIFI
const char* password =  "RodrigoValRobson2021";    //senha da rede wifi
//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "dashboardiot.com.br";   //servermqtt
const char* mqttUser = "emqx_test";              //user servermqtt
const char* mqttPassword = "emqx_test";      //password servermqtt
const int mqttPort = 1883;                     //port

int ultimoEnvioMQTT = 0;
 
WiFiClient espClient;
PubSubClient client(espClient);

#define sub1 "botao1"
#define sub2 "botao2"
#define sub3 "botao3"
#define sub4 "botao4"
#define sub5 "botao5"
#define sub6 "botao6"
#define sub7 "botao7"
#define sub8 "botao8"

#define pub1 "estadodobotao1"
#define pub2 "estadodobotao2"
#define pub3 "estadodobotao3"
#define pub4 "estadodobotao4"
#define pub5 "estadodobotao5"
#define pub6 "estadodobotao6"
#define pub7 "estadodobotao7"
#define pub8 "estadodobotao8"

float luminosidade = analogRead(LDR);
float val = analogRead(LDR);
 
void setup() {
 
  Serial.begin(115200);
  pinMode(Rele1, OUTPUT);
  pinMode(Rele2, OUTPUT);
  pinMode(Rele3, OUTPUT);
  pinMode(Rele4, OUTPUT);
  pinMode(Rele5, OUTPUT);
  pinMode(Rele6, OUTPUT);
  pinMode(Rele7, OUTPUT);
  pinMode(Rele8, OUTPUT);

  pinMode(Botao1, INPUT_PULLUP);
  pinMode(Botao2, INPUT_PULLUP);
  pinMode(Botao3, INPUT_PULLUP);
  pinMode(Botao4, INPUT_PULLUP);
  pinMode(Botao5, INPUT_PULLUP);
  pinMode(Botao6, INPUT_PULLUP);
  pinMode(Botao7, INPUT_PULLUP);
  pinMode(Botao8, INPUT_PULLUP);

  pinMode(wifiLed, OUTPUT);
  pinMode(LDR,INPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(Rele1, HIGH);
  digitalWrite(Rele2, HIGH);
  digitalWrite(Rele3, HIGH);
  digitalWrite(Rele4, HIGH);
  digitalWrite(Rele5, HIGH);
  digitalWrite(Rele6, HIGH);
  digitalWrite(Rele7, HIGH);
  digitalWrite(Rele8, HIGH);
    
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);
 
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
  client.setCallback(callback);
 
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
  dht.begin();
 }
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

    if (strstr(topic, sub1)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele1, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao1 = 0;
      client.publish(pub1, "0");
    } 
    else {
      digitalWrite(Rele1, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao1 = 1;
      client.publish(pub1, "1");
   }    
 }
    else if ( strstr(topic, sub2)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele2, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao2 = 0;
      client.publish(pub2, "0");
    } 
    else {
      digitalWrite(Rele2, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao2 = 1;
      client.publish(pub2, "1");
   }
 }
    else if ( strstr(topic, sub3)) {
    for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele3, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao3 = 0;
      client.publish(pub3, "0");
    } 
    else {
      digitalWrite(Rele3, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao3 = 1;
      client.publish(pub3, "1");
    }
  }
    else if ( strstr(topic, sub4)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele4, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao4 = 0;
      client.publish(pub4, "0");
    } 
    else {
      digitalWrite(Rele4, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao4 = 1;
      client.publish(pub4, "1");
    }
  }
    if (strstr(topic, sub5)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele1, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao1 = 0;
      client.publish(pub5, "0");
    } 
    else {
      digitalWrite(Rele1, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao1 = 1;
      client.publish(pub5, "1");
    }    
  }
    else if ( strstr(topic, sub6)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele2, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao2 = 0;
      client.publish(pub6, "0");
    } 
    else {
      digitalWrite(Rele2, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao2 = 1;
      client.publish(pub6, "1");
    }
  }
    else if ( strstr(topic, sub7)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele3, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao3 = 0;
      client.publish(pub7, "0");
    } 
    else {
      digitalWrite(Rele3, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao3 = 1;
      client.publish(pub7, "1");
    }
  }
    else if ( strstr(topic, sub8)) {
    for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(Rele4, LOW);   // Turn the LED on (Note that LOW is the voltage level
      EstadoDoBotao4 = 0;
      client.publish(pub8, "0");
    } 
    else  {
      digitalWrite(Rele4, HIGH);  // Turn the LED off by making the voltage HIGH
      EstadoDoBotao4 = 1;
      client.publish(pub8, "1");
    }
  }
    else  {
  Serial.println("unsubscribed topic");
  }
}

//função pra reconectar ao servido MQTT
void reconect() {
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
     } 
   else {
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
    reconect();
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

  char MSGUmidadeMQTT[10];
  char MSGTemperaturaMQTT[10];
  char MSGLuminosidadeMQTT[10];
  
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  
  if (isnan(temperatura) || isnan(umidade))  {
    #ifdef DEBUG
    Serial.println("Falha na leitura do DHT22...");
    #endif
  } 
  else  {
    #ifdef DEBUG
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.print(" \n"); //quebra de linha
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
    #endif
    
  sprintf(MSGUmidadeMQTT,"%f",umidade);
  client.publish("casa/umidade", MSGUmidadeMQTT);
   
  sprintf(MSGTemperaturaMQTT,"%f",temperatura);
  client.publish("casa/temperatura", MSGTemperaturaMQTT);

  sprintf(MSGLuminosidadeMQTT,"%f",luminosidade);
  client.publish("casa/luminosidade", MSGLuminosidadeMQTT);

  }
}
