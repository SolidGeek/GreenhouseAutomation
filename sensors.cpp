#include "sensors.h"

// Measurements

sensor_data_t sensor_data = {0};

float temp_inside[3] = {0.0};
float hum_inside[3] = {0.0};
float temp_outside = 0.0;
float hum_outside = 0.0;
float wind_speed = 0.0;
boolean rain_state = false;


int dht_pins[DHT_COUNT] = { 5, 2, 3, 4 };
DHT * dht[DHT_COUNT];

void sensors_init()
{
    pinMode(pin_rain, INPUT_PULLUP);
    pinMode(pin_wind, INPUT_PULLUP);
    pinMode(pin_doors, INPUT_PULLUP);

    for (int n = 0; n < DHT_COUNT; n++){
        dht[n] = new DHT(dht_pins[n], DHT_TYPE); // define a sensor at pin 'n' with sensor type DHT11
        dht[n]->begin();
    }
}



void sensor_dht_read(){

    // Reading temperature or humidity takes about 250 milliseconds!
    float hum = dht[DHT_SENSOR_OUTSIDE]->readHumidity();
    float temp = dht[DHT_SENSOR_OUTSIDE]->readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(hum) || isnan(temp) ) {
      return;
    }

    sensor_data.temp_outside = temp;
    sensor_data.hum_outside = hum;
   
}

void sensor_rain_read(){


}

void sensor_wind_read(){

    
}
