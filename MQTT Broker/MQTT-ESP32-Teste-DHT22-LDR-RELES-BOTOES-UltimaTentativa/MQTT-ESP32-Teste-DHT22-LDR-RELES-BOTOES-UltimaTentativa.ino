//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT, Temperatura, Umidade e Relés
//Autor : Robson Brasil
//Versão : 18
//Última Modificação : 17/08/2021
//Obs.: Os Relés não estão funcionando no protocolo MQTT

#include <WiFi.h>
#include <PubSubClient.h>
#include "esp_adc_cal.h"
 
#define INTERVALO_ENVIO       1000
 
#define DEBUG

#define LampadaRobson 23   //pino de saida para acionamento do LampadaRobson
#define Cooler 22   //pino de saida para acionamento do Cooler
#define Bluetooth 21  //D21
#define LampadaVal 19  //D19
#define LampadaKinha 18  //D18

/* Definições do BOTÃO */
#define SwitchPin1  13  //D13
#define SwitchPin2  12  //D12
#define SwitchPin3  14  //D14
#define SwitchPin4  27  //D27
#define SwitchPin5  33  //D33

/* Definições do Estado do RELÉ */
int toggleState_1  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 1
int toggleState_2  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 2
int toggleState_3  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 3
int toggleState_4  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5
int toggleState_5  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5
 
int wifiFlag       = 0;
 
//informações da rede WIFI
const char* ssid = "RVR 2,4GHz";                 //SSID da rede WIFI
const char* password =  "RodrigoValRobson2021";    //senha da rede wifi
 
//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "broker.hivemq.com";   //servermqtt
//const char* mqttUser = "emqx_test";              //user servermqtt
//const char* mqttPassword = "emqx_test";      //password servermqtt
const int mqttPort = 1883;                     //port

//tópicos que serão assinados
#define mqttTopicSub  "casa/sala/LampadaRobson"        
#define mqttTopicSub  "casa/quarto/robson/LampadaRobson"

/* Defines do MQTT */
#define ESP32-MinhaCasa   "Lampada/LigarLampadaQuartoRobson"
#define ESP32-MinhaCasa   "Cooler/LigarCoolerQuartoRobson"
#define ESP32-MinhaCasa   "LigarBluetoothQuartoRobson"
#define ESP32-MinhaCasa   "LigarLampadaQuartoVal"
#define ESP32-MinhaCasa   "LigarLampadaQuartoKinha"

/* Definições do Acionamento do RELÉ */
void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(LampadaRobson, LOW); // Relé 1 Ligado
              toggleState_1 = 0;
//              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(LampadaRobson, HIGH); // Relé 1 Desligado
              toggleState_1 = 1;
//              Serial.println("Device1 OFF");
              }
             delay(1);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(Cooler, LOW); // Relé 2 Ligado
              toggleState_2 = 0;
//              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(Cooler, HIGH); // Relé 2 Desligado
              toggleState_2 = 1;
//              Serial.println("Device2 OFF");
              }
             delay(1);
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(Bluetooth, LOW); // Relé 3 Ligado
              toggleState_3 = 0;
//              Serial.println("Device3 ON");
              }
             else{
              digitalWrite(Bluetooth, HIGH); // Relé 3 Desligado
              toggleState_3 = 1;
//              Serial.println("Device3 OFF");
              }
             delay(1);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(LampadaVal, LOW); // Relé 4 Ligado
              toggleState_4 = 0;
//              Serial.println("Device4 ON");
              }
             else{
              digitalWrite(LampadaVal, HIGH); // Relé 4 Desligado
              toggleState_4 = 1;
//              Serial.println("Device4 OFF");
              }
             delay(1);
      break;
      case 5: 
             if(toggleState_5 == 1){
              digitalWrite(LampadaKinha, LOW); // Relé 5 Ligado
              toggleState_5 = 0;
//              Serial.println("Device5 ON");
              }
             else{
              digitalWrite(LampadaKinha, HIGH); // Relé 5 Desligado
              toggleState_5 = 1;
//              Serial.println("Device5 OFF");
              }
             delay(1);
      break;
          default : break;      
      }  
}
/* Definições do Botão com Internet */
void with_internet(){
    //Controle de interruptor manual Com Internet
    if (digitalRead(SwitchPin1) == LOW){
      delay(100);
      relayOnOff(1); 
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(100);
      relayOnOff(2);      
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(100);
      relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(100);
      relayOnOff(4);
    }
    else if (digitalRead(SwitchPin5) == LOW){
      delay(200);
      relayOnOff(5); 
    }
}
 
/* Definições do Botão sem Internet */
void without_internet(){
    //Controle de interruptor manual Sem Internet
    if (digitalRead(SwitchPin1) == LOW){
      delay(150);
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(150);
      relayOnOff(2);
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(150);
      relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(150);
      relayOnOff(4);
    }
   else if (digitalRead(SwitchPin5) == LOW){
      delay(150);
      relayOnOff(5);      
    }
}
 
int ultimoEnvioMQTT = 0;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
    /* Configuração do pino ligado ao RELE como output 
       e inicialização do mesmo em LOW */
  pinMode(LampadaRobson, OUTPUT);
  pinMode(Cooler, OUTPUT);
  pinMode(Bluetooth, OUTPUT);
  pinMode(LampadaVal, OUTPUT);
  pinMode(LampadaKinha, OUTPUT);

  //Durante a partida, todos os relés devem Iniciar DESLIGADOS
  digitalWrite(LampadaRobson, HIGH);
  digitalWrite(Cooler, HIGH);
  digitalWrite(Bluetooth, HIGH);
  digitalWrite(LampadaVal, HIGH);
  digitalWrite(LampadaKinha, HIGH);

  digitalWrite(SwitchPin1, LOW);
  digitalWrite(SwitchPin2, LOW);
  digitalWrite(SwitchPin3, LOW);
  digitalWrite(SwitchPin4, LOW);
  
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);
  
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
  //subscreve no tópico
  client.subscribe(mqttTopicSub);
 }
void callback(char* topic, byte* payload, unsigned int length) {
  //armazena strMSG recebida em uma string
  payload[length] = '\0';
  String strMSG = String((char*)payload);
 
    /* toma ação dependendo da string recebida */
        if (strMSG.equals("1"))
    {
        digitalWrite(LampadaRobson, LOW);
        Serial.print("RELÉ 1 aceso mediante comando MQTT");
    }
        if (strMSG.equals("1"))
    {
        digitalWrite(Cooler, LOW);
        Serial.print("RELÉ 2 aceso mediante comando MQTT");
    }
        if (strMSG.equals("1"))
    {
        digitalWrite(Bluetooth, LOW);
        Serial.print("RELÉ 3 aceso mediante comando MQTT");
    }
        if (strMSG.equals("1"))
    {
        digitalWrite(LampadaVal, LOW);
        Serial.print("RELÉ 4 aceso mediante comando MQTT");
    }    
    if (strMSG.equals("1"))
    {
        digitalWrite(LampadaKinha, LOW);
        Serial.print("RELÉ 5 aceso mediante comando MQTT");
    }
 
        if (strMSG.equals("0"))
    {
        digitalWrite(LampadaRobson, HIGH);    
        Serial.print("RELÉ 1 apagado mediante comando MQTT"); 
    }
        if (strMSG.equals("0"))
    {
        digitalWrite(Cooler, HIGH);    
        Serial.print("RELÉ 2 apagado mediante comando MQTT"); 
    }
        if (strMSG.equals("0"))
    {
        digitalWrite(Bluetooth, HIGH);    
        Serial.print("RELÉ 3 apagado mediante comando MQTT"); 
    }
        if (strMSG.equals("0"))
    {
        digitalWrite(LampadaVal, HIGH);    
        Serial.print("RELÉ 4 apagado mediante comando MQTT"); 
    }
        if (strMSG.equals("0"))
    {
        digitalWrite(LampadaKinha, HIGH);    
        Serial.print("RELÉ 5 apagado mediante comando MQTT"); 
    }
 
  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
  #endif
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
    reconect();
  }
 
  //envia a cada X segundos
  if ((millis() - ultimoEnvioMQTT) > INTERVALO_ENVIO)

  client.loop();
}
