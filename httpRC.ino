// httpRC_ESP8266-01 Programm ("Firmware") für den ESP8266 (ESP-01)
//
// Das ESP8266-01 µC Board wird als Webserver konfiguriert, der den PiKoder/SSC über UART
// ansteuert. Der Webserver ist über einen Access Point erreichbar. 

// Das Programm basiert auf dem HTTP 1.1 Webserver als AccessPoint (s.u.)

// Dieses Programm wird mit der Arduino IDE (Version > 1.6.8) kompiliert und an den µC übertragen
// werden, wenn man hier unter Datei -> Voreinstellungen -> Additional Boards Manager URLs:
// "http://arduino.esp8266.com/staging/package_esp8266com_index.json" eingibt.
// Danach eingeben:
// Werkzeuge -> Platine -> Boards Manager -> esp8266 installieren
// Und dann einstellen:
// Platine: "Generic ESP8266 module"
// Flash Mode: "QIO"
// Flash Frequency: "40MHz"
// Upload Using: "Serial"
// CPU Frequency: "80MHz"
// Flash Size: "1M (64K SPIFFS)"
// Upload Speed "115200"
// Port: "COM40" (kann variieren)
// Programmer: "Arduino Gemma"
//
// Weitere Informationen auf www.makerprojekte.de und www.pikoder.de.

/*--------------------------------------------------
HTTP 1.1 Webserver as AccessPoint for ESP8266 
for ESP8266 adapted Arduino IDE

by Stefan Thesen 08/2015 - free for anyone

Does HTTP 1.1 with defined connection closing.
Handles empty requests in a defined manner.
Handle requests for non-exisiting pages correctly.

This demo allows to switch two functions:
Function 1 creates serial output and toggels GPIO2
Function 2 just creates serial output.

Serial output can e.g. be used to steer an attached
Arduino, Raspberry etc.
--------------------------------------------------*/

#include <ESP8266WiFi.h>

const char* ssid = "ESP-Accesspoint";
const char* password = "12345678";  // set to "" for open access point w/o passwortd

unsigned long ulReqcount;

boolean debug = false;

// Create an instance of the server on Port 80
WiFiServer server(80);

void setup() 
{
  // setup globals
  ulReqcount=0; 
  
  // start serial
  Serial.begin(9600);
  delay(10);
  
  // AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.begin();
}

void loop() 
{ 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
  
  // Wait until the client sends some data
  if (debug) Serial.println("new client");
  unsigned long ultimeout = millis()+250;
  while(!client.available() && (millis()<ultimeout) )
  {
    delay(1);
  }
  if(millis()>ultimeout) 
  { 
    if (debug) Serial.println("client connection time-out!");
    return; 
  }
  
  // Read the first line of the request
  String sRequest = client.readStringUntil('\r');
  if (debug) Serial.println(sRequest);
  client.flush();
  
  // stop client, if request is empty
  if(sRequest=="")
  {
    if (debug) Serial.println("empty request! - stopping client");
    client.stop();
    return;
  }
  
  // get path; end of path is either space or ?
  // Syntax is e.g. GET /?pin=MOTOR1STOP HTTP/1.1
  String sPath="",sParam="", sCmd="";
  String sGetstart="GET ";
  int iStart,iEndSpace,iEndQuest;
  iStart = sRequest.indexOf(sGetstart);
  if (iStart>=0)
  {
    iStart+=+sGetstart.length();
    iEndSpace = sRequest.indexOf(" ",iStart);
    iEndQuest = sRequest.indexOf("?",iStart);
    
    // are there parameters?
    if(iEndSpace>0)
    {
      if(iEndQuest>0)
      {
        // there are parameters
        sPath  = sRequest.substring(iStart,iEndQuest);
        sParam = sRequest.substring(iEndQuest,iEndSpace);
      }
      else
      {
        // NO parameters
        sPath  = sRequest.substring(iStart,iEndSpace);
      }
    }
  }
  
  ///////////////////////////////////////////////////////////////////////////////
  // output parameters to serial, you may connect e.g. an Arduino and react on it
  ///////////////////////////////////////////////////////////////////////////////
  if(sParam.length()>0)
  {
    int iEqu=sParam.indexOf("=");
    if(iEqu>=0)
    {
      sCmd = sParam.substring(iEqu+1,sParam.length());
      if (debug) Serial.println(sCmd);
    }
  }
  
  
  ///////////////////////////
  // format the html response
  ///////////////////////////
  String sResponse,sHeader;
  
  ////////////////////////////
  // 404 for non-matching path
  ////////////////////////////
  if(sPath!="/")
  {
    sResponse="<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
    
    sHeader  = "HTTP/1.1 404 Not found\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
  ///////////////////////
  // format the html page
  ///////////////////////
  else
  {
    ulReqcount++;
    sResponse  = "<html><head><title>httpRC</title>";
    sResponse += "<style>.button{-webkit-appearance: none; -webkit-border-radius:0px; width:200px; height:1.7em; font-size:2em;}></style>";
    sResponse += "<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\">";
    sResponse += "</head><body><font color=\"#000000\"><body bgcolor=\"#d0d0f0\">";
    sResponse += "<div  style=\"position:fixed; left; top: 5%; padding:1%\">";
    sResponse += "<a href=\"?pin=fForward\"><button class=\"button\">forward</button></a><a href=\"?pin=fForward\"></div>";
    sResponse += "<div  style=\"position:fixed; right:0%; top:5%; padding:1%\">";
    sResponse += "<a href=\"?pin=fLeft\"><button class=\"button\">left</button></a><a href=\"?pin=fLeft\"></div>";
    sResponse += "<div  style=\"position:fixed; left; top: 32%; padding:1%\">";
    sResponse += "<a href=\"?pin=fStop\"><button class=\"button\">stop</button></a><a href=\"?pin=fStop\"></div>";
    sResponse += "<div  style=\"position:fixed; right:0%; top:32%; padding:1%\">";
    sResponse += "<a href=\"?pin=fStraight\"><button class=\"button\">straight</button></a><a href=\"?pin=fStraight\"></div>";
    sResponse += "<div  style=\"position:fixed; left; top: 60%; padding:1%\">";
    sResponse += "<a href=\"?pin=fBackward\"><button class=\"button\">backward</button></a><a href=\"?pin=fBackward\"></div>";
    sResponse += "<div  style=\"position:fixed; right:0%; top:60%; padding:1%\">"; 
    sResponse += "<a href=\"?pin=fRight\"><button class=\"button\">right</button></a><a href=\"?pin=fRight\"></div>";

    //////////////////////
    // react on parameters
    //////////////////////
    if (sCmd.length()>0)
    {
      // write received command to html page
      // if (debug) sResponse += "Kommando:" + sCmd + "<BR>";
      
      // evaluate commands
      if(sCmd.indexOf("fForward")>=0)
      {
        Serial.write(0xFF);
        Serial.write(0x01);
        Serial.write(0xFE);       
      }
      else if(sCmd.indexOf("fStop")>=0)
      {
        Serial.write(0xFF);
        Serial.write(0x01);
        Serial.write(0x7F);       
      }
      else if(sCmd.indexOf("fBackward")>=0)
      {
        Serial.write(0xFF);
        Serial.write(0x01);
        Serial.write(0x00);       
      }
      else if(sCmd.indexOf("fLeft")>=0)
      {
        Serial.write(0xFF);
        Serial.write(0x00);
        Serial.write(0x00);       
      }
      else if(sCmd.indexOf("fStraight")>=0)
      {
        Serial.write(0xFF);
        Serial.write(0x00);
        Serial.write(0x7F);       
      }
      else if(sCmd.indexOf("fRight")>=0)
      {
        Serial.write(0xFF);
        Serial.write(0x00);
        Serial.write(0xFE);       
      }
    }
    
    sResponse += "</body></html>";
    
    sHeader  = "HTTP/1.1 200 OK\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
  
  // Send the response to the client
  client.print(sHeader);
  client.print(sResponse);

  // and stop the client
  client.stop();
  if (debug) Serial.println("Client disonnected");
}

