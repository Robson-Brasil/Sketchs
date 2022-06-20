//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT, Temperatura, Umidade, Luminosidade, Relés e Botões
//Autor : Robson Brasil
//Versão : 3
//Última Modificação : 16/08/2021

#include "esp_adc_cal.h"
#include <WiFi.h>
#include <PubSubClient.h>

/* Definições do RELE */
#define LampadaRobson 23
#define Cooler 22
#define Bluetooth 21
#define LampadaVal 19
#define LampadaKinha 18

const char* SSID = "RVR 2,4GHz"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "RodrigoValRobson2021"; // Senha da rede WI-FI que deseja se conectar
 
const char* BROKER_MQTT = "broker.hivemq.com"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
//const char* mqttUser = "emqx_test";
//const char* mqttPassword = "emqx_test";

/* Defines do MQTT */
#define ESP32-MinhaCasa  "/Lampada/LigarLampadaQuartoRobson"
#define ESP32-MinhaCasa  "/Cooler/LigarCoolerQuartoRobson"
#define ESP32-MinhaCasa  "/LigarBluetoothQuartoRobson"
#define ESP32-MinhaCasa  "/LigarLampadaQuartoVal"
#define ESP32-MinhaCasa  "/LigarLampadaQuartoKinha"

//Variáveis e objetos globais
WiFiClient espClient;          // Cria o objeto espClient
PubSubClient client(espClient);  // Instancia o Cliente MQTT passando o objeto espClient
 
#define MQTT_CLIENT                  "ESP32ClientGoogleAssistant"     //id mqtt (para identificação de sessão)
                                  //IMPORTANTE: este deve ser único no broker (ou seja, 
                                  //            se um client MQTT tentar entrar com o mesmo 
                                  //            id de outro já conectado ao broker, o broker 
                                  //            irá fechar a conexão de um deles).


void initWiFi(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);
  
/* Definições do LED de Conexão do WI-FI */
#define wifiLed      2   //D2

/* Definições do Estado do RELÉ */
int toggleState_1  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 1
int toggleState_2  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 2
int toggleState_3  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 3
int toggleState_4  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5
int toggleState_5  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5

int wifiFlag       = 0;

void initWiFi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconnectWiFi();
}
 
/* Função: inicializa parâmetros de conexão MQTT(endereço do 
 *         broker, porta e seta função de callback)
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
 

/* Função de setup */
void setup() 
{
    Serial.begin(115200);  
 
    /* Configuração do pino ligado ao RELE como output 
       e inicialização do mesmo em LOW */
  pinMode(LampadaRobson, OUTPUT);
  digitalWrite(LampadaRobson, HIGH);

  pinMode(Cooler, OUTPUT);
  digitalWrite(Cooler, HIGH);

  pinMode(Bluetooth, OUTPUT);
  digitalWrite(Bluetooth, HIGH);

  pinMode(LampadaVal, OUTPUT);
  digitalWrite(LampadaVal, HIGH);

  pinMode(LampadaKinha, OUTPUT);
  digitalWrite(LampadaKinha, HIGH);
 
  pinMode(wifiLed, OUTPUT);

}


/* Função: verifica o estado das conexões WiFI e ao broker MQTT. 
 *         Em caso de desconexão (qualquer uma das duas), a conexão
 *         é refeita.
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
/* Função: reconecta-se ao WiFi
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
void reconnectWiFi(void) 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
          
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
      
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}


/* Função: função de callback 
 *         esta função é chamada toda vez que uma informação de 
 *         um dos tópicos subescritos chega)
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
  
void callback(char* topic, byte* payload, unsigned int length) {
  //armazena strMSG recebida em uma sring
  payload[length] = '\0';
  String strstrMSG = String((char*)payload);
 
  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
  #endif
  
    Serial.print("Chegou a seguinte string via MQTT: ");
    Serial.println(strMSG);
    
    /* toma ação dependendo da string recebida */
        if (strMSG.equals("1"))
    {
        digitalWrite(LampadaRobson, LOW);
        Serial.print("LampadaRobson aceso mediante comando MQTT");
    }
        if (strMSG.equals("1"))
    {
        digitalWrite(Cooler, LOW);
        Serial.print("Cooler aceso mediante comando MQTT");
    }
        if (strMSG.equals("1"))
    {
        digitalWrite(Bluetooth, LOW);
        Serial.print("Bluetooth aceso mediante comando MQTT");
    }
        if (strMSG.equals("1"))
    {
        digitalWrite(LampadaVal, LOW);
        Serial.print("LampadaVal aceso mediante comando MQTT");
    }    
        if (strMSG.equals("1"))
    {
        digitalWrite(LampadaKinha, LOW);
        Serial.print("LampadaKinha aceso mediante comando MQTT");
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
    }
void initMQTT(void) {
  client.setServer(mqttServer, mqttPort);
}
  client.setCallback(callback);
 
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
 
  if (client.connect("ESP32ClientGoogleAssistant", mqttUser, mqttPassword )) {
      #ifdef DEBUG
      Serial.println("Conectado");  
      #endif
     } 
  else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000); //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void reconnectMQTT(void) 
{
 //Enquanto estiver desconectado
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
    #endif
     
    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("ESP32ClientGoogleAssistant", mqttUser, mqttPassword) :
                     client.connect("ESP32ClientGoogleAssistant");
 
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
      delay(5000);
      
 }
  }
} 

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

  
/* Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
 *         em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
 
/* Loop principal */
void loop() 
{
   if (!client.connected()) {
    reconect();
  }
 
   client.loop();
}
