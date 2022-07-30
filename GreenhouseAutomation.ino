#include <SerialTransfer.h>
#include "params.h"
#include "sensors.h"
#include "control.h"

SerialTransfer uart_transfer;

void setup() {

  Serial.begin(115200);
  Serial.println("Greenhouse Booting...");

  Serial3.begin(115200);
  uart_transfer.begin(Serial3);

  control_init();
  sensors_init();

  pinMode( pin_cabinet_open, INPUT_PULLUP );
  pinMode( pout_light_control, OUTPUT );

}

void loop() {

  if( ! digitalRead( pin_cabinet_open )){
    digitalWrite( pout_light_control, HIGH );
  }else{
    digitalWrite( pout_light_control, LOW );
  }

  // Run system
  sensor_run();
  control_run();

  // Send data to WiFi app
  uart_transfer.sendDatum( sensor_data );

  delay(100);
  
}
