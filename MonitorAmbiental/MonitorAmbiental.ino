//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT, Monitor Metereológico
//Autor : Robson Brasil
//Versão : 3
//Última Modificação : 16/08/2021

// Bibliotecas ------------------------------------------
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <DHT.h>
#include <SPIFFS.h>

// Wi-Fi ------------------------------------------------
const char*   ssid                    = "RVR 2,4GHz";
const char*   password                = "RodrigoValRobson2021";

// Constantes -------------------------------------------
// DHT22
const byte    DHT_PIN                 = 16;
// LDR
const byte    LDR_PIN                 = 36;
// Broker
const char*   MQTT_SERVER             = "dashboardiot.com.br";
// Porta
const int     MQTT_PORT               = 1883;
// Cliente
const char*   MQTT_CLIENT             = "IeCMonitor";
// Topico temperatura
const char*   MQTT_TEMPERATURE_TOPIC  = "IeCMonitor/temperatura";
// Topico umidade
const char*   MQTT_HUMIDITY_TOPIC     = "IeCMonitor/umidade";
// Topico iluminancia
const char*   MQTT_LIGHTING_TOPIC     = "IeCMonitor/iluminacao";
// Topico atualizacao
const char*   MQTT_UPDATE_TOPIC       = "IeCMonitor/atualizacao";
// URL da Data/Hora
const char*   HTTP_TIME_URL           = "https://internetecoisas.com.br/exemplo/iec85.php";
// Fuso horario
const int8_t  TIME_ZONE               = -3;
// Intervalo atualizacao dados
const int     DATA_UPDATE_INT         = 10;
// Intervalo atualizacao Data/Hora - sugestao: 28800 = 8h
const int     DATETIME_INT            = 60;

const char* mqttUser = "emqx_test";
const char* mqttPassword = "emqx_test";

// Variaveis globais ------------------------------------
// Data/Hora
time_t        dateTime                = 0;
// Proxima atualizacao de dados
time_t        nextDataUpdate          = 0;
// Temperatura
String        temperature;
// Umidade
String        humidity;
// Iluminação
String        lighting;
// Atualização
String        lastUpdate;

// Instancias -------------------------------------------
WiFiClientSecure espClient;
PubSubClient client(espClient, MQTT_SERVER, MQTT_PORT);
WebServer server(80);
DHT dht(DHT_PIN, DHT22);

// Funcoes Genericas ------------------------------------
time_t iso8601DateTime(String s, const int8_t tz = 0) {
  // Converte String ISO8601 em time_t
  TimeElements te;
  te.Year   = s.substring( 0,  4).toInt() - 1970;
  te.Month  = s.substring( 5,  7).toInt();
  te.Day    = s.substring( 8, 10).toInt();
  te.Hour   = s.substring(11, 13).toInt();
  te.Minute = s.substring(14, 16).toInt();
  te.Second = s.substring(17, 19).toInt();
  time_t t  = makeTime(te);
  t-= s.substring(19, 22).toInt() * 3600; // Ajusta p/ UTC
  t+= tz * 3600;                          // Ajusta p/ local
  return t;
}

String dateTimeStr(time_t t, const int8_t tz = 0, const bool flBr = true) {
  // Retorna time_t como "yyyy-mm-dd hh:mm:ss" ou "dd/mm/yyyy hh:mm:ss"
  if (t == 0) {
    return F("N/A");
  }
  t += tz * 3600;
  String sFn;
  if (flBr) {
    // dd/mm/yyyy hh:mm:ss
    sFn = "";
    if (day(t) < 10) {
      sFn += '0';
    }
    sFn += String(day(t)) + '/';
    if (month(t) < 10) {
      sFn += '0';
    }
    sFn += String(month(t)) + '/' + String(year(t)) + ' ';
  } else {
    // yyyy-mm-dd hh:mm:ss
    sFn = String(year(t)) + '-';
    if (month(t) < 10) {
      sFn += '0';
    }
    sFn += String(month(t)) + '-';
    if (day(t) < 10) {
      sFn += '0';
    }
    sFn += String(day(t)) + ' ';
  }
  if (hour(t) < 10) {
    sFn += '0';
  }
  sFn += String(hour(t)) + ':';
  if (minute(t) < 10) {
    sFn += '0';
  }
  sFn += String(minute(t)) + ':';
  if (second(t) < 10) {
    sFn += '0';
  }
  sFn += String(second(t));
  return sFn;
}

String dateTimeISO8601(const time_t &t, const int8_t tz = 0) {
  // Convert time_t em String ISO8601
  String s = dateTimeStr(t, tz, false);
  s.replace(" ", "T");
  if (tz == 0) {
    // Fuso UTC
    s += "Z";
  } else {
    // Fuso especificado
    s = s + ((tz < 0) ? "-" : "+") +
            ((tz < 10) ? "0" : "") +
            String(abs(tz)) +
            ":00";
  }
  return s;
}

String ipStr(const IPAddress &ip) {
  // Retorna IPAddress como String "n.n.n.n"
  String s = "";
  for (byte b = 0; b < 3; b++) {
    s += String((ip >> (8 * b)) & 0xFF) + ".";
  }
  s += String(((ip >> 8 * 3)) & 0xFF);
  return s;
}

int analogAverage(const byte &a) {
  // Retornar média de 100 leituras da porta analógica
  long l = 0;
  for (byte b = 0; b < 100; b++) {
    l += analogRead(a);
    delay(1);
  }
  return l / 100;
}

void log(const String &s) {
  // Gera log no Monitor Serial
  Serial.println(dateTimeStr(dateTime, TIME_ZONE) + " " + s);
}

void reconnect() {
  // Conecta ao Broker
  while (!client.connected()) {
    log("Connectando Broker MQTT...");
    // Conecta
    if (client.connect(MQTT_CLIENT + String(random(1e5)))) {
      log("Conectado");
    } else {
      // Falha na conexao
      log("Falha");
      delay(5000);
    }
  }
}

time_t timeHTTP() {
  // Obtem data/hora via HTTP
  HTTPClient http;
  http.begin(HTTP_TIME_URL);
  int httpCode = http.GET();
  String s = http.getString();
  http.end();
  s.trim();
  while (s[0] > 127) {
    s.remove(0, 1);
  }

  if (httpCode != HTTP_CODE_OK) {
    // Falha na requisicao HTTP
    log("Falha obtendo Data/Hora");
    setSyncInterval(10);
    return 0;
  }

  // Retorna Data/Hora
  dateTime = iso8601DateTime(s);
  log("Data/Hora atualizada");
  setSyncInterval(DATETIME_INT);
  return dateTime;
}

// Processa requisoes HTTP ------------------------------
void handleGetData() {
  // Obtem leitura dos sensores
  String s = temperature + "&" + humidity + "&" + lighting + "&" + lastUpdate;
  server.send(200, "text/plain", s);
  log("getData " + ipStr(server.client().remoteIP()) + " " + s);
}

void handleDashboard() {
  // Pagina padrao com Dashboard
  File file = SPIFFS.open("/Dashboard.htm", "r");
  if (file) {
    server.streamFile(file, "text/html");
    file.close();
    log("Dashboard " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, "text/plain", "ERROR 500 - Dashboard");
    log("Dashboard error");
  }
}

// Setup ------------------------------------------------
void setup() {
  // Incializa
  Serial.begin(115200);
  log("--- Monitor Ambiental ---");

  // SPIFFS
  if (SPIFFS.begin()) {
    log("SPIFFS ok");
  } else {
    log("SPIFFS falha");
    while(true);
  }

  // Conecta Wi-Fi
  log("Conectando WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  log("Conectado IP " + ipStr(WiFi.localIP()));

  // Instrui Cliente a ignorar assinatura do Servidor na conexao segura
  espClient.setInsecure();

  // Servidor HTTP
  server.on("/getData", handleGetData);
  server.onNotFound(handleDashboard);
  server.begin();

  // DHT22
  dht.begin();

  // Define Data/Hora
  setSyncProvider(timeHTTP);

  // Pronto
  log("Pronto");
}

// Loop -------------------------------------------------
void loop() {
  // Processa requisicao HTTP
  server.handleClient();

  // Processa conexao ao Broker
  if (client.connected()) {
    client.loop();
  } else {
    reconnect();
  }

  // Atualiza data/hora
  dateTime = now();

  if (dateTime > nextDataUpdate) {
    // Atualizar leitura dos sensores
    temperature = String(dht.readTemperature(), 1);
    humidity = String(dht.readHumidity(), 1);
    lighting = String(analogAverage(LDR_PIN) * 100L / 4095);
    lastUpdate = dateTimeISO8601(dateTime);

    if (isnan(dht.readTemperature())) {
      // Falha na leitura
      log("Falha lendo DHT22");
    } else {
      // Publica dados no broker
      client.publish(MQTT::Publish(MQTT_TEMPERATURE_TOPIC, temperature)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_HUMIDITY_TOPIC, humidity)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_LIGHTING_TOPIC, lighting)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_UPDATE_TOPIC, lastUpdate)
                      .set_retain(true));
      log("Dados atualizados t=" + temperature + "\tu=" + humidity + "\tl=" + lighting);
    }

    // Proxima atualizacao
    nextDataUpdate = dateTime + DATA_UPDATE_INT;
  }
}
