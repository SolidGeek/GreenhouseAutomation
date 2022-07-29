#include "sensors.h"

// Measurements
sensor_data_t sensor_data = {0};
uint32_t sensor_timer; 

int dht_pins[DHT_COUNT] = { 5, 2, 3, 4 };
SimpleDHT22 * dht[DHT_COUNT];

void sensors_init()
{
    pinMode(pin_rain, INPUT_PULLUP);
    pinMode(pin_wind, INPUT_PULLUP);
    pinMode(pin_doors, INPUT_PULLUP);

    for (int n = 0; n < DHT_COUNT; n++){
        dht[n] = new SimpleDHT22(dht_pins[n]); // define a sensor at pin 'n' with sensor type DHT11
    }
}

void sensor_run(){

  if( millis() - sensor_timer  > 5000 ){
    
    sensor_timer = millis();

    // Read all DHT Sensors
    for(uint8_t i = 0; i < 4; i++ ){
  
      sensor_dht_read(i);
    }

  }

}

void sensor_dht_read( uint8_t num ){

    // Reading temperature or humidity takes about 250 milliseconds!
    float temp = 0;
    float hum = 0;

    int status = dht[num]->read2(&temp, &hum, NULL);

    if( status != SimpleDHTErrSuccess ){
      Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(status)); Serial.println(num);
      return;
    }

    if( num == DHT_SENSOR_OUTSIDE ){
      sensor_data.temp_outside = temp;
      sensor_data.hum_outside = hum;
    }else{
      sensor_data.temp_inside[num-1] = temp;
      sensor_data.hum_inside[num-1] = hum;
    }

}

void sensor_rain_read(){


}

void sensor_wind_read(){

    
}
