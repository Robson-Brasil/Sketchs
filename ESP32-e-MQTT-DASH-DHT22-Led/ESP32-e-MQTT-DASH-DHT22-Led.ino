//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT, Temperatura, Umidade, Luminosidade, Relés e Botões
//Autor : Robson Brasil
//Versão : 3
//Última Modificação : 05/08/202

#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* SSID = "RVR 2,4GHz"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "RodrigoValRobson2021"; // Senha da rede WI-FI que deseja se conectar
 
const char* BROKER_MQTT = "192.168.15.30"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
const char* mqttUser = "Robson Brasil";
const char* mqttPassword = "LoboAlfa";
   
//Variáveis e objetos globais
WiFiClient espClient;          // Cria o objeto espClient
PubSubClient MQTT(espClient);  // Instancia o Cliente MQTT passando o objeto espClient
 
/* Definições do RELE */
#define RelayPin1   23  //D23
#define RelayPin2   22  //D22
#define RelayPin3   21  //D21
#define RelayPin4   19  //D19
#define RelayPin5   18  //D18
#define RelayPin6    5  //D5
#define RelayPin7   25  //D25
#define RelayPin8   26  //D26
/* Definições do BOTÃO */
#define SwitchPin1  13  //D13
#define SwitchPin2  12  //D12
#define SwitchPin3  14  //D14
#define SwitchPin4  27  //D27
#define SwitchPin5  33  //D33
#define SwitchPin6  32  //D32
#define SwitchPin7  15  //D15
#define SwitchPin8   4  //D4
/* Definições do LED de Conexão do WI-FI */
#define wifiLed      2   //D2

/* Defines do MQTT */
#define sub1  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor1"
#define sub2  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor2"
#define sub3  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor3"
#define sub4  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor4"
#define sub5  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor5"
#define sub6  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor6"
#define sub7  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor7"
#define sub8  "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor8"

#define pub9   "ESP32-MinhaCasa/QuartoRobson/Temperatura"
#define pub10  "ESP32-MinhaCasa/QuartoRobson/Umidade"
#define pub11  "ESP32-MinhaCasa/QuartoRobson/Luminosidade"

#define MQTT_CLIENT              "IoTESP32"     //id mqtt (para identificação de sessão)
                                  //IMPORTANTE: este deve ser único no broker (ou seja, 
                                  //            se um client MQTT tentar entrar com o mesmo 
                                  //            id de outro já conectado ao broker, o broker 
                                  //            irá fechar a conexão de um deles).
 

/* Definições do Estado do RELÉ */
int toggleState_1  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 1
int toggleState_2  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 2
int toggleState_3  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 3
int toggleState_4  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5
int toggleState_5  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5
int toggleState_6  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 6
int toggleState_7  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 7
int toggleState_8  = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 8
int wifiFlag       = 0;
/* Definicoes do sensor DHT22 */
#define DHT22PIN    16  //GPIO que está ligado o pino de dados do sensor
#define DHTTYPE DHT22   //Sensor em utilização: DHT22
//#define LDR
#define LDR         36  //Sensor LDR
/* Definições do Acionamento do RELÉ */
void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // Relé 1 Ligado
              toggleState_1 = 0;
//              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // Relé 1 Desligado
              toggleState_1 = 1;
//              Serial.println("Device1 OFF");
              }
             delay(1);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); // Relé 2 Ligado
              toggleState_2 = 0;
//              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // Relé 2 Desligado
              toggleState_2 = 1;
//              Serial.println("Device2 OFF");
              }
             delay(1);
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(RelayPin3, LOW); // Relé 3 Ligado
              toggleState_3 = 0;
//              Serial.println("Device3 ON");
              }
             else{
              digitalWrite(RelayPin3, HIGH); // Relé 3 Desligado
              toggleState_3 = 1;
//              Serial.println("Device3 OFF");
              }
             delay(1);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(RelayPin4, LOW); // Relé 4 Ligado
              toggleState_4 = 0;
//              Serial.println("Device4 ON");
              }
             else{
              digitalWrite(RelayPin4, HIGH); // Relé 4 Desligado
              toggleState_4 = 1;
//              Serial.println("Device4 OFF");
              }
             delay(1);
      break;
      case 5: 
             if(toggleState_5 == 1){
              digitalWrite(RelayPin5, LOW); // Relé 5 Ligado
              toggleState_5 = 0;
//              Serial.println("Device5 ON");
              }
             else{
              digitalWrite(RelayPin5, HIGH); // Relé 5 Desligado
              toggleState_5 = 1;
//              Serial.println("Device5 OFF");
              }
             delay(1);
      break;
      case 6: 
             if(toggleState_6 == 1){
              digitalWrite(RelayPin6, LOW); // Relé 6 Ligado
              toggleState_6 = 0;
//              Serial.println("Device6 ON");
              }
             else{
              digitalWrite(RelayPin6, HIGH); // Relé 6 Desligado
              toggleState_6 = 1;
//              Serial.println("Device6 OFF");
              }
             delay(1);
      break;
      case 7: 
             if(toggleState_7 == 1){
              digitalWrite(RelayPin7, LOW); // Relé 7 Ligado
              toggleState_7 = 0;
//             Serial.println("Device7 ON");
              }
             else{
              digitalWrite(RelayPin7, HIGH); // Relé 7 Desligado
              toggleState_7 = 1;
//              Serial.println("Device7 OFF");
              }
             delay(1);
      break;
      case 8: 
             if(toggleState_8 == 1){
              digitalWrite(RelayPin8, LOW); // Relé 8 Ligado
              toggleState_8 = 0;
//              Serial.println("Device8 ON");
              }
             else{
              digitalWrite(RelayPin8, HIGH); // Relé 8 Desligado
              toggleState_8 = 1;
//              Serial.println("Device8 OFF");
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
    else if (digitalRead(SwitchPin6) == LOW){
      delay(100);
      relayOnOff(6);      
    }
    else if (digitalRead(SwitchPin7) == LOW){
      delay(200);
      relayOnOff(7);
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(100);
      relayOnOff(8);
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
    else if (digitalRead(SwitchPin6) == LOW){
      delay(150);
      relayOnOff(6);
   }
    else if (digitalRead(SwitchPin7) == LOW){
      delay(150);
      relayOnOff(7);
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(150);
      relayOnOff(8);
    }
}

/* Variaveis, constantes e objetos globais */
DHT dht(DHT22PIN, DHTTYPE);
 
/* Prototypes */
float faz_leitura_temperatura(void);
float faz_leitura_umidade(void);
float luminosidade = analogRead(LDR);

void initWiFi(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);
/*
 * Implementações
 */
 /* Função: faz a leitura de temperatura (sensor DHT22)
 * Parametros: nenhum
 * Retorno: temperatura (graus Celsius)
 */
float faz_leitura_temperatura(void)
{
    float t = dht.readTemperature();
    float result;
     
    if (! (isnan(t)) )
        result = t;
    else
        result = -99.99;
    return result;
}
/* Função: faz a leitura de umidade relativa do ar (sensor DHT22)
 * Parametros: nenhum
 * Retorno: umidade (0 - 100%)
 */
float faz_leitura_umidade(void)
{
    float h = dht.readHumidity();    
    float result;
     
    if (! (isnan(h)) )
        result = h;
    else
        result = -99.99;
    return result;
}

float val = analogRead(LDR);
/* Função: inicializa e conecta-se na rede WI-FI desejada
*  Parâmetros: nenhum
*  Retorno: nenhum
*/

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
 
void initMQTT(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
/* Função: função de callback 
 *         esta função é chamada toda vez que uma informação de 
 *         um dos tópicos subescritos chega)
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
 
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
  
    /* obtem a string do payload recebido */
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
 
    Serial.print("Chegou a seguinte string via MQTT: ");
    Serial.println(msg);
    
    /* toma ação dependendo da string recebida */
        if (msg.equals("1"))
    {
        digitalWrite(RelayPin1, LOW);
        Serial.print("RELÉ 1 aceso mediante comando MQTT");
    }
        if (msg.equals("1"))
    {
        digitalWrite(RelayPin2, LOW);
        Serial.print("RELÉ 2 aceso mediante comando MQTT");
    }
        if (msg.equals("1"))
    {
        digitalWrite(RelayPin3, LOW);
        Serial.print("RELÉ 3 aceso mediante comando MQTT");
    }
        if (msg.equals("1"))
    {
        digitalWrite(RelayPin4, LOW);
        Serial.print("RELÉ 4 aceso mediante comando MQTT");
    }    
    if (msg.equals("1"))
    {
        digitalWrite(RelayPin5, LOW);
        Serial.print("RELÉ 5 aceso mediante comando MQTT");
    }
        if (msg.equals("1"))
    {
        digitalWrite(RelayPin6, LOW);
        Serial.print("RELÉ 6 aceso mediante comando MQTT");
    }
        if (msg.equals("1"))
    {
        digitalWrite(RelayPin7, LOW);
        Serial.print("RELÉ 7 aceso mediante comando MQTT");
    } 
        if (msg.equals("1"))
    {
        digitalWrite(RelayPin8, LOW);
        Serial.print("RELÉ 8 aceso mediante comando MQTT");
    } 
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin1, HIGH);    
        Serial.print("RELÉ 1 apagado mediante comando MQTT"); 
    }
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin2, HIGH);    
        Serial.print("RELÉ 2 apagado mediante comando MQTT"); 
    }
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin3, HIGH);    
        Serial.print("RELÉ 3 apagado mediante comando MQTT"); 
    }
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin4, HIGH);    
        Serial.print("RELÉ 4 apagado mediante comando MQTT"); 
    }
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin5, HIGH);    
        Serial.print("RELÉ 5 apagado mediante comando MQTT"); 
    }
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin6, HIGH);    
        Serial.print("RELÉ 6 apagado mediante comando MQTT"); 
    }
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin7, HIGH);    
        Serial.print("RELÉ 7 apagado mediante comando MQTT"); 
    }
        if (msg.equals("0"))
    {
        digitalWrite(RelayPin8, HIGH);    
        Serial.print("RELÉ 8 apagado mediante comando MQTT"); 
    }
}
  
/* Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
 *         em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
 
void reconnectMQTT(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(MQTT_CLIENT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(sub1);
            MQTT.subscribe(sub2);
            MQTT.subscribe(sub3);
            MQTT.subscribe(sub4);
            MQTT.subscribe(sub5);
            MQTT.subscribe(sub6);
            MQTT.subscribe(sub7);
            MQTT.subscribe(sub8);

        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
 
/* Função: verifica o estado das conexões WiFI e ao broker MQTT. 
 *         Em caso de desconexão (qualquer uma das duas), a conexão
 *         é refeita.
 * Parâmetros: nenhum
 * Retorno: nenhum
 */
void VerificaConexoesWiFIEMQTT(void)

{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
      
     reconnectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
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
/* Função de setup */
void setup() 
{
    Serial.begin(115200);  
 
    /* Configuração do pino ligado ao RELE como output 
       e inicialização do mesmo em LOW */
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);
  pinMode(RelayPin6, OUTPUT);
  pinMode(RelayPin7, OUTPUT);
  pinMode(RelayPin8, OUTPUT);

  //Durante a partida, todos os relés devem Iniciar DESLIGADOS
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  digitalWrite(RelayPin5, HIGH);
  digitalWrite(RelayPin6, HIGH);
  digitalWrite(RelayPin7, HIGH);
  digitalWrite(RelayPin8, HIGH);    

  digitalWrite(SwitchPin1, LOW);
  digitalWrite(SwitchPin2, LOW);
  digitalWrite(SwitchPin3, LOW);
  digitalWrite(SwitchPin4, LOW);
  digitalWrite(SwitchPin5, LOW);
  digitalWrite(SwitchPin6, LOW);
  digitalWrite(SwitchPin7, LOW);
  digitalWrite(SwitchPin8, LOW);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);
  pinMode(SwitchPin6, INPUT_PULLUP);
  pinMode(SwitchPin7, INPUT_PULLUP);
  pinMode(SwitchPin8, INPUT_PULLUP);

  pinMode(wifiLed, OUTPUT);

  pinMode(LDR,INPUT);
 
    /* Inicializacao do sensor de temperatura */
    dht.begin();  
 
    /* Inicializa a conexao wi-fi */
    initWiFi();
 
    /* Inicializa a conexao ao broker MQTT */
    initMQTT();
} 
/* Loop principal */
void loop() 
{
    char temperatura_str[10] = {0};
    char umidade_str[10]     = {0};
     
    /* garante funcionamento das conexões WiFi e ao broker MQTT */
    VerificaConexoesWiFIEMQTT();
 
    /* Compoe as strings a serem enviadas pro dashboard (campos texto) */
    sprintf(temperatura_str,"%.2fC", faz_leitura_temperatura());
    sprintf(umidade_str,"%.2f", faz_leitura_umidade());
 
    /*  Envia as strings ao dashboard MQTT */
    MQTT.publish(pub9, temperatura_str);
    MQTT.publish(pub10, umidade_str);
//    MQTT.publish(pub10, analogRead(LDR));
   
    /* keep-alive da comunicação com broker MQTT */
    MQTT.loop();
 
    /* Refaz o ciclo após 2 segundos */
    delay(2000);
}
