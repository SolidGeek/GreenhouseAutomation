#ifndef CONTROL_H_
#define CONTROL_H_

#include "params.h"
#include "sensors.h"

#define MAX_TASKS 16 

// References for controller
extern float ref_temp[3];
extern float ref_hum[3];
extern float temp_margin;
extern float control_margin;

// P control gains
const float temp_gain = 1.0;
const float hum_gain = 1.0;

// Setup of all output pins
void control_init();

// Runs automation (if enabled) and checks for new tasks
void control_run();

void control_manuel();

void control_manager();

// Function to initiate a control task asynchronous
void control_task( void *start, void *end, uint8_t zone, uint16_t runtime );

// Functions to call using "control_task"
void control_stop( uint8_t zone );
void control_open( uint8_t zone );
void control_close( uint8_t zone );

// Temperature and humidity controller
void control_auto();

#endif
