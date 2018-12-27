// httpRC_ESP8266-01 Programm ("Firmware") für den ESP8266 (ESP-01)
//
// Das ESP8266-01 µC Board wird als Webserver konfiguriert, der den PiKoder/SSC über UART
// ansteuert. Der Webserver ist über einen Access Point erreichbar. Der Sketch steuert per
// default über Kanal 1 (CH1) die Lenkung und über Kanal 2 (CH2) die Geschwindigkeit.  

// Dieses Programm wird mit der Arduino IDE Version 1.8.5) kompiliert und an den µC übertragen

// Weitere Informationen auf www.makerprojekte.de und www.pikoder.de.

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "index.h"  // our HTML webpage contents with javascripts

const char* ssid = "ESP-Accesspoint"; // AP definitions
const char* password = "";  // set to "" for open access point w/o passwortd

boolean debug = false;  // Output via UART

ESP8266WebServer server(80);  // create an instance of the server on Port 80

void handleRoot() {
  String htmlPageFrame = MAIN_page; // read HTML contents
  server.send(200, "text/html", htmlPageFrame); // send to web page
}

void handleCMD() {
  if (server.hasArg("CH1")) {
        Serial.write(0xFF);
        Serial.write(0x00);    
        writeSpeed("CH1");
  }      
  if (server.hasArg("CH2")) {
        Serial.write(0xFF);
        Serial.write(0x01);    
        writeSpeed("CH2");
  } 
  server.send(200, "text/html", " ");
}

void setup() {
  
  // start serial
  Serial.begin(9600);
  delay(10);
  
  // AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.on("/", HTTP_GET, handleRoot); 
  server.on("/CMD", HTTP_GET, handleCMD);
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();
}

void loop() { 
  server.handleClient();
}

void writeSpeed (String Channel) {
  String sCmd =server.arg(Channel);
  int chValue = 0x7F;  // equals neutral 
  if (sCmd == "1") chValue = 0xFE;
  if (sCmd == "-1") chValue = 0x00; 
  Serial.write(chValue);    
}    

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
