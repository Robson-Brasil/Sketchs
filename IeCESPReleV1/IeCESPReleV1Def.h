/*******************************************************************************
**  CANAL INTERNET E COISAS                                                   **
**  IeCESPRele V1.0 Definitions                                               **
**  09/2018 - Andre Michelon                                                  **
*******************************************************************************/

/*******************************************************************************
* LIBRARIES
*******************************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <FS.h>
#include <EEPROM.h>

/*******************************************************************************
* CONSTANT DEFINITION
*******************************************************************************/
// Device Name
const String    DEVICE_NAME                     = "ESPRele";

// Build Information
const char      BUILD[]                         = __DATE__ " " __TIME__;

// Web Interface Username
const char      WWW_USERNAME[]                  = "esprele";

// System Password
const char      PASSWORD[]                      = "Rele8266";

// WebServer Port
const byte      WEBSERVER_PORT                  = 80;

// DNS Server Port
const byte      DNSSERVER_PORT                  = 53;

// WebServer Headers
const char*     WEBSERVER_HEADER_KEYS[]         = {"User-Agent", "Cookie"};

// Directory Max Number of Files
const byte      DIRECTORY_MAX_FILES             = 20;

// Config Parameters
const byte      CFG_REFERENCE                   =  0;
const byte      CFG_PWD_ALLWAYS_NEEDED          = 35 + CFG_REFERENCE;
const byte      CFG_SOFTAP_ALLWAYS_ON           =  1 + CFG_PWD_ALLWAYS_NEEDED;
const byte      CFG_WIFI_SSID                   =  1 + CFG_SOFTAP_ALLWAYS_ON;
const byte      CFG_WIFI_PWD                    = 35 + CFG_WIFI_SSID;
const byte      CFG_TOTAL_LENGHT                = 35 + CFG_WIFI_PWD;

// Pin Mapping
const byte      RELAY_PIN                       =  3;

// Error Codes
const byte      ERR_SPIFFS_BEGIN                =  1;
const byte      ERR_MDNS_BEGIN                  =  2;
const byte      ERR_WEB_HOME_FILEOPEN           =  3;
const byte      ERR_WEB_RELAY_FILEOPEN          =  4;
const byte      ERR_WEB_FILELIST_FILEOPEN       =  5;
const byte      ERR_WEB_CONFIG_FILEOPEN         =  6;
const byte      ERR_WEB_REBOOT_FILEOPEN         =  7;
const byte      ERR_WEB_NOTFOUND_FILEOPEN       =  8;
const byte      ERR_MSG_TEST                    = 99;

/*******************************************************************************
* ENTITIES
*******************************************************************************/
// Web Server
ESP8266WebServer  server(WEBSERVER_PORT);

// DNS Server
DNSServer dnsServer;

// HTTP Client
HTTPClient        http;

/*******************************************************************************
* VARIABLES
*******************************************************************************/
// WiFi Status
wl_status_t wifiStatus          = WL_DISCONNECTED;

// WiFi SoftAP flag
boolean softAPActive = false;

// Version Control
String            swVersion     = "1.0";
String            fsVersion     = "";
