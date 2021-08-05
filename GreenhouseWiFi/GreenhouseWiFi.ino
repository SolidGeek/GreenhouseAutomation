// Import required libraries
#include "ESP8266WiFi.h"
#include "ESP8266mDNS.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "SerialTransfer.h"
#include "FS.h"

typedef struct{
  float temp_outside;
  float hum_outside;
  float temp_inside[3];
  float hum_inside[3];
} sensor_data_t;


typedef enum{
  CMD_UNVALID = 0,
  CMD_RESTART = 1,
  CMD_ENABLE_AUTO,
  CMD_ENABLE_MANUEL,
  CMD_OPEN_ALL,
  CMD_CLOSE_ALL,
  CMD_OPEN_ZONE,
  CMD_CLOSE_ZONE,
  CMD_SET_TEMPERATURE,
  CMD_SET_HUMIDITY,
} command_t;

sensor_data_t sensor_data;

// Replace with your network credentials
const char* ssid = "NOKIA-7CF1";
const char* password = "Solvang121";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
SerialTransfer uart_transfer;

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  uart_transfer.begin(Serial);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("greenhouse")) {
    Serial.println("Error setting up MDNS responder!");
  }
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){

    DynamicJsonDocument json(1024);

    json["temp_outside"] = sensor_data.temp_outside;
    json["hum_outside"] = sensor_data.hum_outside;

    json["temp_inside"][0] = sensor_data.temp_inside[0];
    json["temp_inside"][1] = sensor_data.temp_inside[1];
    json["temp_inside"][2] = sensor_data.temp_inside[2];

    json["hum_inside"][0] = sensor_data.hum_inside[0];
    json["hum_inside"][1] = sensor_data.hum_inside[1];
    json["hum_inside"][2] = sensor_data.hum_inside[2];

    char stringout[200];
    serializeJson(json, stringout);
    request->send(200, "application/json", stringout);
    
  });

  // Start server
  server.begin();
}
 
void loop(){
  // Read sensor data from Mega
  if( uart_transfer.available()  ){
    // Read struct into command_buffer
    uart_transfer.rxObj( sensor_data );
  }
  
}
