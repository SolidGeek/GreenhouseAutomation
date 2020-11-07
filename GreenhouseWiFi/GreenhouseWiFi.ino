#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "FS.h"

const char* ssid     = "Waoo4920_VR48";
const char* password = "Solvang121";
const char* device = "GreenhouseWiFi";

AsyncWebServer server(80);

IPAddress ip(192, 168, 1, 154);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);   //optional

void setup() {

  Serial.begin(9600);
  delay(10);
  
  if(!SPIFFS.begin()){
    Serial.println("Failed to initialise SPIFFS");
  }

  Serial.println("WiFi configured");
  WiFi.disconnect();
  WiFi.hostname(device); 
  WiFi.config(ip, subnet, gateway, dns);
  
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(1000)));
  });

  server.begin();
}

void loop() {

  
}
