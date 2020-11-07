#ifndef PARAMS_H_
#define PARAMS_H_

#include "Arduino.h"


#define MODE_MANUEL 0
#define MODE_AUTO 1

/* ----- Constants ----- */

// Manual control pins
const uint8_t pin_open[3]       = {26,32,38};
const uint8_t pin_close[3]      = {27,33,39};

// Window control pins
const uint8_t pout_open[3]      = {28,34,40};
const uint8_t pout_close[3]     = {29,35,41};
const uint8_t pout_kip_open[3]  = {30,36,42};
const uint8_t pout_kip_close[3] = {31,37,43};

// Other control pins
const uint8_t pout_temp_control           = 44; // To turn on and off a heatpump
const uint8_t pout_light_control          = 45; // To turn on and off the light

// Interface pins
const uint8_t pin_change_mode             = 18;
const uint8_t pin_change_zone             = 17;
const uint8_t pin_value_increase          = 15;
const uint8_t pin_value_decrease          = 16;
const uint8_t pin_change_setting          = 14;
const uint8_t pin_cabinet_ope             = 10;


// Sensor pins
const uint8_t pin_temp_inside[3] = {2,3,4}; 
const uint8_t pin_temp_outside = 5;
const uint8_t pin_rain = 6;
const uint8_t pin_wind = 7;
const uint8_t pin_doors = 8;



const float tempMargin = 1.0;
const float humidityMargin = 5.0; // Procent
const int processDelay = 3000; // 5 sekunder
const long changeDelay = 900000; // 15 minutter. 900000
const int checkWindDelay = 30000; // Hvert 1 minut
const float maxWindSpeed = 15.00; // 15 m/s


const uint8_t zones[] = {0,1,2};
const uint8_t zones_num = 3; // Total numbers of zones

extern boolean system_state;
extern boolean window_state;
extern boolean top_window_state;

#endif // PARAMS_H_
