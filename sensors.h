#ifndef SENSORS_H_
#define SENSORS_H_

#define DHT_COUNT 4
#define DHT_SENSOR_OUTSIDE 0
#define DHT_SENSOR_ZONE_1 1
#define DHT_SENSOR_ZONE_2 2
#define DHT_SENSOR_ZONE_3 3

#include <Arduino.h>
#include <SimpleDHT.h>

#include "params.h"

typedef struct{
  float temp_outside;
  float temp_inside[3];
  float hum_outside;
  float hum_inside[3];
  float wind_speed;
  bool rain_state;
} sensor_data_t;


// Sensor variables global to all that includes sensors.h
extern sensor_data_t sensor_data;

// Setup of all output pins
void sensors_init();

// Read temperature and humidity
void sensor_dht_read( uint8_t num );

// Read all sensors
void sensor_run();

// Read rain sensor state
void sensor_rain_read();

// Measure wind speed
void sensor_wind_read();


#endif
