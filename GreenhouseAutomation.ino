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

  sensors_init();
  control_init();

  
      
}

void loop() {

  
  sensor_dht_read();
  uart_transfer.sendDatum( sensor_data );

  control_run();


  delay(1000);
  
}
