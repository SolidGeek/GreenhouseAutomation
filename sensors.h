#ifndef SENSORS_H_
#define SENSORS_H_

#define DHT_TYPE DHT22
#define DHT_COUNT 4
#define DHT_SENSOR_OUTSIDE 0x0
#define DHT_SENSOR_ZONE_1 0x1
#define DHT_SENSOR_ZONE_2 0x2
#define DHT_SENSOR_ZONE_3 0x3

#include <Arduino.h>
#include <DHT.h>

#include "params.h"

typedef struct{
  float temp_outside;
  float hum_outside;
  float temp_inside[3];
  float hum_inside[3];
} sensor_data_t;

extern sensor_data_t sensor_data;

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
