/*******************************************************************************
**  CANAL INTERNET E COISAS                                                   **
**  IeCESPRele V1.0 Library - Generic Functions                               **
**  09/2018 - Andre Michelon                                                  **
*******************************************************************************/

void hold(const unsigned int &ms) {
  // Non blocking delay
  unsigned long m = millis();
  while (millis() - m < ms) {
    yield();
  }
}

void errMsg(const byte &e) {
  // Show error messages, waits for 10s and reboot
  Serial.println("ERRO;" + String(e));
  hold(10000);
  yield();
  ESP.restart();
}

void log(const String &type, const String &msg, const byte &err = 0) {
  // Generate log and display error code if any
  Serial.println(type + ";" + msg);
  // Check for error code
  if (err != 0) {
    errMsg(err);
  }
}

boolean pwdNeeded() {
  // Return CFG_PWD_ALLWAYS_NEEDED config value
  return EEPROM.read(CFG_PWD_ALLWAYS_NEEDED);
}

boolean softAPOn() {
  // Return CFG_SOFTAP_ALLWAYS_ON config value
  return EEPROM.read(CFG_SOFTAP_ALLWAYS_ON);
}

String reference() {
  // Return Reference config parameter
  String sFn = "";
  for (byte bFn = CFG_REFERENCE; bFn < CFG_SOFTAP_ALLWAYS_ON; bFn++) {
    if (EEPROM.read(bFn) == 0) {
      break;
    }
    sFn += char(EEPROM.read(bFn));
  }
  return sFn;
}

String wifiSSID() {
  // Return WiFi SSID config parameter
  String sFn = "";
  for (byte bFn = CFG_WIFI_SSID; bFn < CFG_WIFI_PWD; bFn++) {
    if (EEPROM.read(bFn) == 0) {
      break;
    }
    sFn += char(EEPROM.read(bFn));
  }
  return sFn;
}

String wifiPwd() {
  // Return WiFi password config parameter
  String sFn = "";
  for (byte bFn = CFG_WIFI_PWD; bFn < CFG_TOTAL_LENGHT; bFn++) {
    if (EEPROM.read(bFn) == 0) {
      break;
    }
    sFn += char(EEPROM.read(bFn));
  }
  return sFn;
}

String configStr() {
  // Return Config parameters
  return "Ref="  + reference() +
        " Aut="  + (pwdNeeded() ? "S" : "N") +
        " AP="   + (softAPOn()  ? "S" : "N") +
        " SSID=" + wifiSSID();
}

void getSPIFFSBuild() {
  // Inicialize SPIFFS build variable
  File file = SPIFFS.open("/SPIFFSBuild.txt", "r");
  if (file) {
    file.setTimeout(100);
    fsVersion = file.readString();
    file.close();
  } else {
    fsVersion = "N/A";
  }
}

String hexStr(const unsigned long &h, const byte &l = 8) {
  // Return value as Hexadecimal String
  String sFn;
  sFn= String(h, HEX);
  sFn.toUpperCase();
  sFn = ("00000000" + sFn).substring(sFn.length() + 8 - l);
  return sFn;
}

String deviceID() {
  // Return the Device ID
  return DEVICE_NAME + hexStr(ESP.getChipId());
}

String ipStr(const IPAddress &ip) {
  // Return IPAddress as String "n.n.n.n"
  String sFn = "";
  for (byte bFn = 0; bFn < 3; bFn++) {
    sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
  }
  sFn += String(((ip >> 8 * 3)) & 0xFF);
  return sFn;
}

String wifiStatusStr() {
  // Return WiFi Status as a readable String
  String s;
  switch (WiFi.status()) {
    case WL_IDLE_STATUS:
      s = "Ocioso";
      break;
    case WL_NO_SSID_AVAIL:
      s = "SSID indisponível";
      break;
    case WL_SCAN_COMPLETED:
      s = "Scan concluído";
      break;
    case WL_CONNECTED:
      s = "Conectado";
      break;
    case WL_CONNECT_FAILED:
      s = "Falha de conexão";
      break;
    case WL_CONNECTION_LOST:
      s = "Conexão perdida";
      break;
    case WL_DISCONNECTED:
      s = "Desconectado";
      WiFi.reconnect();
      break;
    default:
      s = "N/D";
      break;
  }
  return s;
}

boolean chkWebAuth(){
  // Check for Web Interface credencials
  if (server.authenticate(WWW_USERNAME, PASSWORD)) {
    // Authenticated
    return true;
  } else {
    // Need Username and Password
    log("WebAut", "ERRO falha na autenticação");
    server.sendHeader("WWW-Authenticate", "Basic realm=\"ESPRele Web Interface\"");
    server.send(401);
    return false;
  }
}

void sortArray(String a[], String &s) {
  // Sort a String Array
  boolean flFn = true;
  String sFn;
  while (flFn) {
    flFn = false;
    byte bFn = 0;
    while(a[bFn + 1] != "") {
      if(a[bFn] > a[bFn + 1]) {
        sFn = a[bFn + 1];
        a[bFn + 1] = a[bFn];
        a[bFn] = sFn;
        flFn = true;
      }
      bFn++;
    }
    bFn = 0;
    s = "";
    while (a[bFn] != "") {
      s += a[bFn];
      bFn++;
    }
  }
}

String fsSpaceStr() {
  // Return information about FileSystem space
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  return  "Total: " +
            String((fs_info.totalBytes) / 1048576.0, 2) + "mb<br>" +
          "Usado: " +
            String(fs_info.usedBytes / 1048576.0, 2) + "mb<br>" +
          "Disponível: " +
            String((fs_info.totalBytes - fs_info.usedBytes) / 1048576.0, 2) + "mb<br>";
}
