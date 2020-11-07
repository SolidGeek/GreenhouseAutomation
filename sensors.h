#ifndef SENSORS_H_
#define SENSORS_H_

#define DHTTYPE DHT22

#include <Arduino.h>
#include <DHT.h>
#include "params.h"

// Sensor variables global to all that includes sensors.h
extern float temp_inside[3];
extern float hum_inside[3];
extern float temp_outside;
extern float hum_outside;
extern float wind_speed;
extern boolean rain_state;

// Setup of all output pins
void sensors_init();

// Read temperature and humidity
void sensor_dht_read();

// Read rain sensor state
void sensor_rain_read();

// Measure wind speed
void sensor_wind_read();

#endif
