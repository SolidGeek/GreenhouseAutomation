// Import required libraries
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <ArduinoJson.h>
#include <SerialTransfer.h>

typedef struct{
  float temp_outside;
  float temp_inside[3];
  float hum_outside;
  float hum_inside[3];
  float wind_speed;
  bool rain_state;
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
const char* pass = "Solvang121";
const char* host = "greenhouse";

// Replace with your own address
const char* handler_url = "https://greenhouse.solidgeek.dk/handler.php";

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
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, pass);
  WiFi.hostname(host);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.print("System ready on: ");
  Serial.println(WiFi.localIP());

  if ( MDNS.begin(host) ) {
      Serial.println("MDNS responder started");
  }
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.on("/uPlot.iife.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/uPlot.iife.min.js");
  });

  server.on("/uPlot.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/uPlot.min.css");
  });
  
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){

    char buf[200];
    generate_json_string(buf);
    
    request->send(200, "application/json", buf);
    
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTAw

  // Start server
  server.begin();

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  
}

void generate_json_string( char * buf ){
  
    DynamicJsonDocument json(1024);

    json["temp_out"] = sensor_data.temp_outside;
    json["hum_out"] = sensor_data.hum_outside;

    json["temp_in"][0] = sensor_data.temp_inside[0];
    json["temp_in"][1] = sensor_data.temp_inside[1];
    json["temp_in"][2] = sensor_data.temp_inside[2];

    json["hum_in"][0] = sensor_data.hum_inside[0];
    json["hum_in"][1] = sensor_data.hum_inside[1];
    json["hum_in"][2] = sensor_data.hum_inside[2];

    json["wind_speed"] = sensor_data.wind_speed;
    json["is_rain"] = sensor_data.rain_state;

    // Control values
    json["output"][0] = 0;
    json["output"][1] = 0;
    json["output"][2] = 0;

    serializeJson(json, buf);
    
}

void loop(){
  // Read sensor data from Mega
  if( uart_transfer.available()  ){
    // Read struct into command_buffer
    uart_transfer.rxObj( sensor_data );

    // Publish data to database
    if(WiFi.status()== WL_CONNECTED){

      char buf[200];
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, handler_url);
      
      http.addHeader("Content-Type", "application/json");

      generate_json_string( buf );
      int httpResponseCode = http.POST( buf );

      // Free resources
      http.end();
    }
  }

  MDNS.update();
  
}
