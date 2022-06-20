//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT, Temperatura, Umidade e Relés
//Autor : Robson Brasil
//Versão : 3
//Última Modificação : 16/08/2021

// Import required libraries
#include "WiFi.h"
#include <WiFiClient.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <AsyncTCP.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <Wire.h>

#define ssid      "RVR 2,4GHz"      // WiFi SSID
#define password  "RodrigoValRobson2021"  // WiFi password
#define DHTTYPE   DHT22           // DHT type (DHT11, DHT22)
#define DHTPIN    16              // Broche du DHT - DHT Pin
const uint8_t GPIOPIN[4] = {23,22,21,19};  // Broches Led - Led Pins
float   t = 0 ;
float   h = 0 ;
float   p = 0;
String  etatGpio[4] = {"OFF","OFF","OFF","OFF"};
String  theme = "bootstrap";

// Création des objets - create Objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
WebServer server(8221);

String getPage(){
  // Renvoi une chaine contenant le code HTML de la page - Return a string containing the HTML code of the page
  String page = "<html charset=UTF-8><head><meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/>";
  page += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  if ( theme == "bootstrap" ) {
    page += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
  } else {
    page += "<link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/";
    page += theme;
    page += "/bootstrap.min.css' rel='stylesheet'>";
  }
  page += "<title>Automação Residencial</title></head><body>";
  page += "<div class='container-fluid'>";
  page +=   "<div class='row'>";
  page +=     "<div class='col-md-12'>";
  page +=       "<h1>Webserver ESP32 + Bootstrap</h1>";
  page +=       "<h3>Mini station m&eacute;t&eacute;o</h3>";
  page +=       "<ul class='nav nav-pills'>";
  page +=         "<li class='active'>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           t;
  page +=           "</span> Temp&eacute;rature</a>";
  page +=         "</li><li>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           h;
  page +=           "</span> Humidit&eacute;</a>";
  page +=         "</li><li>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           p;
  page +=           "</span> Pression atmosph&eacute;rique</a></li>";
  page +=       "</ul>";
  page +=       "<table class='table'>";  // Tableau des relevés
  page +=         "<thead><tr><th>Capteur</th><th>Mesure</th><th>Valeur</th><th>Valeur pr&eacute;c&eacute;dente</th></tr></thead>"; //Entête
  page +=         "<tbody>";  // Contenu du tableau - Table content
  page +=           "<tr><td>DHT22</td><td>Temp&eacute;rature</td><td>"; // Première ligne : température - First line : temperature
  page +=             t;
  page +=             "&deg;C</td><td>";
  page +=             "-</td></tr>";
  page +=           "<tr class='active'><td>DHT22</td><td>Humidit&eacute;</td><td>"; // 2nd ligne : Humidité - 2nd line : humidity
  page +=             h;
  page +=             "%</td><td>";
  page +=             "-</td></tr>";
  page +=           "<tr><td>BMP180</td><td>Pression atmosph&eacute;rique</td><td>"; // 3ème ligne : PA (BMP180) - 3th line : atmospheric pressure
  page +=             p;
  page +=             "mbar</td><td>";
  page +=             "-</td></tr>";
  page +=       "</tbody></table>";
  page +=       "<h3>GPIO</h3>";
  page +=       "<div class='row'>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>23 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[0];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='23' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='23' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>22 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[1];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='22' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='22' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>21 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[2];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='21' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='21' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>19 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[3];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='19' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='19' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=       "</div>";
  page +=   "<div class='row'>";
  page +=     "<div class='col-md-4'>";
  page +=       "<form method='POST' name='selecttheme' id='selecttheme'/>"; 
  page +=       "<input class='span' id='choixtheme' name='theme' type='hidden'>";
  page +=       "<div class='btn-group'>";
  page +=         "<button class='btn btn-default'>Choisir un th&eacute;me</button>";
  page +=         "<button data-toggle='dropdown' class='btn btn-default dropdown-toggle'><span class='caret'></span></button>";
  page +=         "<ul class='dropdown-menu'>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"bootstrap\"); $(\"#selecttheme\").submit()'><a href='#'>Boostrap</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"cerulean\"); $(\"#selecttheme\").submit()'><a href='#'>Cerulean</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"cosmo\"); $(\"#selecttheme\").submit()'><a href='#'>Cosmo</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"cyborg\"); $(\"#selecttheme\").submit()'><a href='#'>Cyborg</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"darkly\"); $(\"#selecttheme\").submit()'><a href='#'>Darkly</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"flatly\"); $(\"#selecttheme\").submit()'><a href='#'>Flatly</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"journal\"); $(\"#selecttheme\").submit()'><a href='#'>Journal</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"lumen\"); $(\"#selecttheme\").submit()'><a href='#'>Lumen</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"paper\"); $(\"#selecttheme\").submit()'><a href='#'>Paper</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"readable\"); $(\"#selecttheme\").submit()'><a href='#'>Readable</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"sandstone\"); $(\"#selecttheme\").submit()'><a href='#'>Sandstone</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"simplex\"); $(\"#selecttheme\").submit()'><a href='#'>Simplex</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"slate\"); $(\"#selecttheme\").submit()'><a href='#'>Slate</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"spacelab\"); $(\"#selecttheme\").submit()'><a href='#'>Spacelab</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"superhero\"); $(\"#selecttheme\").submit()'><a href='#'>Superhero</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"united\"); $(\"#selecttheme\").submit()'><a href='#'>United</a></li>";
  page +=           "<li onclick='$(\"#choixtheme\").val(\"yeti\"); $(\"#selecttheme\").submit()'><a href='#'>Yeti</a></li>";
  page +=         "</ul>";
  page +=       "</div>";
  page +=       "</form></div>";
  page +=       "<div class='col-md-8'>";
  page +=         "<p><a href='http://www.projetsdiy.fr'>Version francaise : www.projetsdiy.fr</p>";
  page +=         "<p><a href='http://www.diyprojects.io'>English version : www.diyprojects.io</p>";
  page +=       "</div>";
  page +=   "</div>"; 
  page += "</div></div></div>";
  page += "</body></html>";
  return page;
}
void handleRoot(){ 
  if ( server.hasArg("theme") ) {
    handleTheme();
  } else if ( server.hasArg("23") ) {
    handle23();
  } else if ( server.hasArg("22") ) {
    handle22();
  } else if ( server.hasArg("21") ) {
    handle21();
  } else if ( server.hasArg("19") ) {
    handle19();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}

void handleTheme(){
  theme = server.arg("theme");
  Serial.println("Update theme : "); Serial.print(theme);  
  server.send ( 200, "text/html", getPage() );
}

void handle23() {
  String D5Value; 
  updateGPIO(0,server.arg("23")); 
}

void handle22() {
  String D6Value; 
  updateGPIO(1,server.arg("22")); 
}

void handle21() {
  String D7Value; 
  updateGPIO(2,server.arg("21")); 
}

void handle19() {
  String D8Value; 
  updateGPIO(3,server.arg("19")); 
}

void updateGPIO(int gpio, String DxValue) {
  Serial.println("");
  Serial.println("Update GPIO "); Serial.print(GPIOPIN[gpio]); Serial.print(" -> "); Serial.println(DxValue);
  
  if ( DxValue == "1" ) {
    digitalWrite(GPIOPIN[gpio], HIGH);
    etatGpio[gpio] = "On";
    server.send ( 200, "text/html", getPage() );
  } else if ( DxValue == "0" ) {
    digitalWrite(GPIOPIN[gpio], LOW);
    etatGpio[gpio] = "Off";
    server.send ( 200, "text/html", getPage() );
  } else {
    Serial.println("Err Led Value");
  }  
}

void setup() {
  for ( int x = 0 ; x < 5 ; x++ ) { 
    pinMode(GPIOPIN[x],OUTPUT);
  }  
  Serial.begin ( 115200 );
  // Initialisation du BMP180 - Init BMP180 sensor
  if ( !bmp.begin() ) {
    Serial.println("BMP180 KO!");
    while(1);
  } else {
    Serial.println("BMP180 OK");
  }
  
  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi - Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  // Connexion WiFi établie - WiFi connexion is OK
  Serial.println ( "" ); 
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  // On branche la fonction qui gère la premiere page / Endpoints for HTTP server
  server.on ( "/", handleRoot );

  server.begin();
  Serial.println ( "HTTP server started" );
  
}

void loop() {
  // Vérifie régulièrement la connexion d'un nouveau client - check for incomming client connections frequently
  server.handleClient();
  t = dht.readTemperature();
  h = dht.readHumidity();
  p = bmp.readPressure() / 100.0F;

  delay(1000);
}
