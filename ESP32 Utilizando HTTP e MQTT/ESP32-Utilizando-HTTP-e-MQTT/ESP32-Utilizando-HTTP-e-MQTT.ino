// Bibliotecas ------------------------------------------
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <TimeLib.h>

// Wi-Fi ------------------------------------------------
const char* ssid = "RVR 2,4GHz";
const char* password = "RodrigoValRobson2021";

// Constantes -------------------------------------------
// RELE1
const byte    RELE1_PIN             = 23;
// Broker
const char*   MQTT_SERVER         = "dashboardiot.com.br";
// Porta
const int     MQTT_PORT           = 1883;
// Cliente
const char*   MQTT_CLIENT         = "IeCESP32";
// Topico RELE1 Get
const char*   MQTT_RELE1_GET        = "IeCRELE1/get";
// Topico RELE1 Set
const char*   MQTT_RELE1_SET        = "IeCRELE1/set";
// URL da Data/Hora
const char*   HTTP_TIME_URL       = "https://internetecoisas.com.br/exemplo/iec85.php";
// Fuso horario
const int8_t  TIME_ZONE           = -3;
// Intervalo atualizacao Data/Hora
const int     DATETIME_INT        = 30; // Utilizar 28800 = 8h

// Variaveis globais ------------------------------------
// Data/Hora
time_t        dateTime            = 0;

// Instancias -------------------------------------------
WiFiClientSecure espClient;
PubSubClient client(espClient, MQTT_SERVER, MQTT_PORT);
WebServer server(8221);

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
void reconnect() {
  // Conecta ao Broker
  while (!client.connected()) {
    Serial.println("Connectando Broker...");
    // Conecta
    if (client.connect(MQTT_CLIENT + String(random(1e5)))) {
      client.set_callback(callback);
      client.subscribe(MQTT_RELE1_SET);
      Serial.println("Conectado");
    } else {
      // Falha na conexao
      Serial.println("Falha");
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
    Serial.println("Falha obtendo Data/Hora");
    setSyncInterval(10);
    return 0;
  }

  // Retorna Data/Hora
  Serial.println("Data/Hora atualizada");
  setSyncInterval(DATETIME_INT);
  return iso8601DateTime(s);
}

// Processa status do RELE1 -------------------------------
char RELE1Status(char c) {
  // Le/Define estado do RELE1
  String s;
  char st;
  boolean flPublish = true;
  if (c == '0') {
    // RELE1 desligado
    digitalWrite(RELE1_PIN, LOW);
    s = "desligado";
    st = '0';
  } else if (c == '1') {
    // RELE1 ligado
    digitalWrite(RELE1_PIN, HIGH);
    s = "ligado";
    st = '1';
  } else if (c == 'x' || c == 'X') {
    // Inverter RELE1
    digitalWrite(RELE1_PIN, !digitalRead(RELE1_PIN));
    s = "invertido para ";
    s += (digitalRead(RELE1_PIN) ? "ligado" : "desligado");
    st = digitalRead(RELE1_PIN) ? '1' : '0';
  } else {
    // Consulta
    s = "consulta";
    st = digitalRead(RELE1_PIN) ? '1' : '0';
    flPublish = false;
  }
  if (flPublish) {
    // Atualiza topico MQTT
    client.publish(MQTT::Publish(MQTT_RELE1_GET, String(st))
                    .set_retain(true));
  }
  // Exibe status
  Serial.println(s);
  return st;
}

// Processa requisoes MQTT ------------------------------
void callback(const MQTT::Publish& pub) {
  // Trata topicos MQTT recebidos
  if (pub.topic() == MQTT_RELE1_SET) {
    Serial.print("[MQTT] ");
    char c = pub.payload_string()[0];
    RELE1Status(c);
  }
}

// Processa requisoes HTTP ------------------------------
void handleRELE1Status() {
  // Le/Define estado do RELE1
  Serial.print("[HTTP] ");
  char c = server.arg("set")[0];
  server.send(200, "text/plain", String(RELE1Status(c)));
}

void handleNotFound() {
  // Pagina padrao com Data/Hora e status do RELE1
  String s =  "<html>"
                "<head>"
                  "<title>ESP32 HTTP Server</title>"
                  "<meta http-equiv=\"Refresh\" content=\"5\">"
                "</head>"
                "<body>"
                  "<h1>ESP32 HTTP Server</h1>"
                  "Data/Hora: " + dateTimeStr(now(), TIME_ZONE) + "<p>"
                  "RELE1: " + (digitalRead(RELE1_PIN) ? "Ligado" : "Desligado") +
                "</body>"
              "</html>";
  server.send(200, "text/html", s);
}

// Setup ------------------------------------------------
void setup() {
  // Incializa
  Serial.begin(115200);
  pinMode(RELE1_PIN, OUTPUT);
  randomSeed(analogRead(0));

  // Conecta Wi-Fi
  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi conectado, IP "); Serial.println(WiFi.localIP());

  // Servidor HTTP
  server.on("/RELE1Status", handleRELE1Status);
  server.onNotFound(handleNotFound);
  server.begin();

  // Define Data/Hora
  setSyncProvider(timeHTTP);
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

  if (now() != dateTime) {
    // Exibe Data/Hora
    dateTime = now();
    Serial.println(dateTimeStr(dateTime, TIME_ZONE));
  }
}
