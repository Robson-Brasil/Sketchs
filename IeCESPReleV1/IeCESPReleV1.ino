/*******************************************************************************
**  CANAL INTERNET E COISAS                                                   **
**  IeCESPRele                                                                **
**  09/2018 - Andre Michelon                                                  **
*******************************************************************************/

/*******************************************************************************
* AUXILIARY FILES
*******************************************************************************/
#include "IeCESPReleV1Def.h"
#include "IeCESPReleV1Lib.h"

/*******************************************************************************
* AUXILIARY FUNCTIONS
*******************************************************************************/
String softwareStr() {
  // Return source file name as String
  return String(__FILE__).substring(String(__FILE__).lastIndexOf("\\") + 1);
}

/*******************************************************************************
* WEB REQUESTS HANDLING
*******************************************************************************/
void handleHome() {
  // Home
  if (!pwdNeeded() || chkWebAuth()) {
    File file = SPIFFS.open("/Home.htm", "r");
    if (file) {
      file.setTimeout(100);
      String s = file.readString();
      file.close();
      // Replace markers
      s.replace("#reference#" , reference());
      s.replace("#serial#"    , hexStr(ESP.getChipId()));
      s.replace("#software#"  , softwareStr());
      s.replace("#swVersion#" , swVersion);
      s.replace("#swBuild#"   , BUILD);
      s.replace("#fsVersion#" , fsVersion.substring(21));
      s.replace("#fsBuild#"   , fsVersion.substring(0, 20));
      s.replace("#sysIP#"     , ipStr(WiFi.localIP()));
      s.replace("#clientIP#"  , ipStr(server.client().remoteIP()));
      s.replace("#userAgent#" , server.header("User-Agent"));
      // Send data
      server.send(200, "text/html", s);
      log("WebHome", "Cliente: " + ipStr(server.client().remoteIP()) +
                      (server.uri() != "/" ? " [" + server.uri() + "]" : ""));
    } else {
      server.send(500, "text/plain", "Home - ERROR 500");
      log("WebHome", "ERRO lendo arquivo", ERR_WEB_HOME_FILEOPEN);
    }
  }
}

void handleRelay() {
  // Relay
  if (!pwdNeeded() || chkWebAuth()) {
    File file = SPIFFS.open("/Relay.htm", "r");
    if (file) {
      file.setTimeout(100);
      String s = file.readString();
      file.close();
      // Replace markers
      s.replace("#reference#" , reference());
      // Send data
      server.send(200, "text/html", s);
      log("WebRelay", "Cliente: " + ipStr(server.client().remoteIP()));
    } else {
      server.send(500, "text/plain", "Relay - ERROR 500");
      log("WebRelay", "ERRO lendo arquivo", ERR_WEB_RELAY_FILEOPEN);
    }
  }
}

void handleRelayStatus() {
  // Relay status
  if (!pwdNeeded() || chkWebAuth()) {
    String s = String(digitalRead(RELAY_PIN));
    server.send(200, "text/plain", s);
    log("WebRelayStatus", "Cliente: " + ipStr(server.client().remoteIP()) +
                          " [" + s + "]");
  }
}

void handleRelaySet() {
  // Set Relay status
  if (!pwdNeeded() || chkWebAuth()) {
    String s = server.arg("set");
    if (s == "1") {
      // Set relay on
      digitalWrite(RELAY_PIN, HIGH);
    } else if (s == "0") {
      // Set relay off
      digitalWrite(RELAY_PIN, LOW);
    }
    server.send(200, "text/plain", String(digitalRead(RELAY_PIN)));
    log("WebRelaySet", "Cliente: " + ipStr(server.client().remoteIP()) +
                        " [" + s + "]");
  }
}

void handleFileList() {
  // File list
  if (!pwdNeeded() || chkWebAuth()) {
    File file = SPIFFS.open("/FileList.htm", "r");
    if (file) {
      file.setTimeout(100);
      String  s = file.readString(),
              sort = "",
              files[DIRECTORY_MAX_FILES];
      file.close();
      Dir dir = SPIFFS.openDir("/");
      byte b = 0;
      while (dir.next()) {
        files[b] = "<li>" + dir.fileName() + " - " + String(dir.fileSize() / 1024.0, 1) + "kb</li>";
        b++;
      }
      // Sort entries
      sortArray(files, sort);
      // Replace markers
      s.replace("#files#"   , "<ul>" + sort + "</ul>");
      s.replace("#fsSpace#" , fsSpaceStr());
      // Send data
      server.send(200, "text/html", s);
      log("WebFileList", "Cliente: " + ipStr(server.client().remoteIP()));
    } else {
      server.send(500, "text/plain", "FileList - ERROR 500");
      log("WebFileList", "ERRO lendo arquivo", ERR_WEB_FILELIST_FILEOPEN);
    }
  }
}

void handleConfig() {
  // Config
  if (chkWebAuth()) {
    File file = SPIFFS.open("/Config.htm", "r");
    if (file) {
      file.setTimeout(100);
      String s = file.readString();
      file.close();
      // Replace markers
      s.replace("#reference#" , reference());
      s.replace("#aut#"       , (pwdNeeded() ? " checked" : ""));
      s.replace("#ap#"        , (softAPOn() ? " checked" : ""));
      s.replace("#ssid#"      , wifiSSID());
      // Send data
      server.send(200, "text/html", s);
      log("WebConfig", "Cliente: " + ipStr(server.client().remoteIP()));
    } else {
      server.send(500, "text/plain", "Config - ERROR 500");
      log("WebConfig", "ERRO lendo arquivo", ERR_WEB_CONFIG_FILEOPEN);
    }
  }
}

void handleConfigSave() {
  // Config Save
  if (chkWebAuth()) {
    // Check for number os fields received
    if (server.args() >= 3 && server.args() <= 5) {
      // Save CFG_PWD_ALLWAYS_NEEDED
      EEPROM.write(CFG_PWD_ALLWAYS_NEEDED, server.arg("aut").toInt());
      // Save CFG_SOFTAP_ALLWAYS_ON
      EEPROM.write(CFG_SOFTAP_ALLWAYS_ON, server.arg("ap").toInt());
      // Save CFG_REFERENCE
      byte b = 0;
      String s = server.arg("reference");
      s.trim();
      if (s == "") {
        s = deviceID();
      }
      for (int i = CFG_REFERENCE; i < CFG_PWD_ALLWAYS_NEEDED; i++) {
        EEPROM.write(i, s[b++]);
      }
      // Save CFG_WIFI_SSID
      b = 0;
      s = server.arg("ssid");
      s.trim();
      for (int i = CFG_WIFI_SSID; i < CFG_WIFI_PWD; i++) {
        EEPROM.write(i, s[b++]);
      }
      // Save CFG_WIFI_PWD
      s = server.arg("pwd");
      s.trim();
      if (s != "") {
        b = 0;
        for (int i = CFG_WIFI_PWD; i < CFG_TOTAL_LENGHT; i++) {
          EEPROM.write(i, s[b++]);
        }
      }
      // Save to flash
      EEPROM.commit();
      server.send(200, "text/html", "<html><meta charset='UTF-8'><script>alert('Configuração salva.');history.back()</script></html>");
      log("WebConfigSave", "Cliente: " + ipStr(server.client().remoteIP()) +
                            " [" + configStr() + "]");
    } else {
      server.send(200, "text/html", "<html><meta charset='UTF-8'><script>alert('Erro de parâmetros.');history.back()</script></html>");
    }
  }
}

void handleReboot() {
  // Reboot
  if (chkWebAuth()) {
    File file = SPIFFS.open("/Reboot.htm", "r");
    if (file) {
      server.streamFile(file, "text/html");
      file.close();
      log("WebReboot", "Cliente: " + ipStr(server.client().remoteIP()));
      hold(100);
      ESP.restart();
    } else {
      server.send(500, "text/plain", "Reboot - ERROR 500");
      log("WebReboot", "ERRO lendo arquivo", ERR_WEB_REBOOT_FILEOPEN);
    }
  }
}

/*******************************************************************************
* SETUP
*******************************************************************************/
void setup() {
  // Serial
  Serial.begin(74880);

  // Slow down
  hold(1000);
  Serial.println();

  // Relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // File System begin
  if (!SPIFFS.begin()) {
    log("Boot", "SPIFFS ERRO", ERR_SPIFFS_BEGIN);
  }

  // Read config
  EEPROM.begin(CFG_TOTAL_LENGHT);

  // FileSystem Version
  getSPIFFSBuild();

  // Starting
  log("Boot", "Iniciando...");
  log("Boot", "Dispositivo: " + deviceID());
  log("Boot", "Serial: " + hexStr(ESP.getChipId()));
  log("Boot", "Software: " + softwareStr());
  log("Boot", "Versão Software: " + String(BUILD) + " " + swVersion);
  log("Boot", "Versão Arquivos: " + fsVersion);
  log("Boot", "Configuração: " + configStr());
  log("Boot", "Causa de Boot: " + ESP.getResetReason());

  // WiFi - Access Point & Station
  log("Boot", "Iniciando WiFi");
  WiFi.hostname(deviceID());
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(true);
  WiFi.mode(softAPOn() ? WIFI_AP_STA : WIFI_STA);
  WiFi.begin(wifiSSID().c_str(), wifiPwd().c_str());
  
  log("Boot", "Conectando WiFi " + wifiSSID());
  byte b = 0;
  while(wifiStatus == WL_DISCONNECTED && b < 60) {
    b++;
    Serial.print(".");
    hold(500);
    wifiStatus = WiFi.status();
  }
  Serial.println();

  if (wifiStatus == WL_CONNECTED) {
    // WiFi connected
    log("Boot", "WiFi Ok Modo Station RSSI " + String(WiFi.RSSI()) + " IP " + ipStr(WiFi.localIP()));
  }

  // Check for SoftAP mode
  if (softAPOn() || wifiStatus != WL_CONNECTED) {
    if (wifiStatus != WL_CONNECTED) {
      // No Wifi connection, enter SoftAP
      WiFi.mode(WIFI_AP);
    }
    WiFi.softAP(deviceID().c_str(), PASSWORD);

    // Set flag
    softAPActive = true;

    // Enable DNS routing
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNSSERVER_PORT, "*", WiFi.softAPIP());

    log("Boot", "WiFi Ok Modo SoftAP");
  }

  // mDNS
  if (!MDNS.begin(deviceID().c_str())) {
    log("Boot", "mDNS ERRO", ERR_MDNS_BEGIN);
  }

  // WebServer
  server.on("/relay", handleRelay);
  server.on("/relayStatus", handleRelayStatus);
  server.on("/relaySet", handleRelaySet);
  server.on("/fileList", handleFileList);
  server.on("/config", handleConfig);
  server.on("/configSave", handleConfigSave);
  server.on("/reboot", handleReboot);
  server.onNotFound(handleHome);
  server.collectHeaders(WEBSERVER_HEADER_KEYS, 2);
  server.begin();
  log("Boot", "Servidor Web Ok");

  // Ready
  log("Boot", "ESPRele pronto");
}

/*******************************************************************************
* LOOP
*******************************************************************************/
void loop() {
  String s;
  // Take care of WatchDog ---------------------------------------------------
  yield();

  // DNS Requests ------------------------------------------------------------
  if (softAPActive) {
    dnsServer.processNextRequest();
  }

  // Web Requests ------------------------------------------------------------
  server.handleClient();

  // WiFi Status -----------------------------------------------------------
  if (WiFi.status() != wifiStatus) {
    wifiStatus = WiFi.status();
    log("WiFi", wifiStatusStr() + " RSSI " + String(WiFi.RSSI()) + " IP " + ipStr(WiFi.localIP()));
  }
}
