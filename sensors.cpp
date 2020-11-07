#include "sensors.h"

// Measurements
float temp_inside[3] = {0.0};
float hum_inside[3] = {0.0};
float temp_outside = 0.0;
float hum_outside = 0.0;
float wind_speed = 0.0;
boolean rain_state = false;

DHT dht0(pin_temp_inside[0], DHTTYPE);
DHT dht1(pin_temp_inside[1], DHTTYPE);
DHT dht2(pin_temp_inside[2], DHTTYPE);
DHT dht3(pin_temp_outside, DHTTYPE);

void sensors_init()
{
    pinMode(pin_rain, INPUT_PULLUP);
    pinMode(pin_wind, INPUT_PULLUP);
    pinMode(pin_doors, INPUT_PULLUP);

    // Init temperature and humidity sensors
    dht0.begin();
    dht1.begin();
    dht2.begin();
    dht3.begin();
}


void sensor_dht_read(){

    // Read outside temperature first:

    // Read all zones:

}

void sensor_rain_read(){


}

void sensor_wind_read(){

    
}