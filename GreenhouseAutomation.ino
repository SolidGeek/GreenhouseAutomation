#include "params.h"
#include "sensors.h"
#include "control.h"
#include "display.h"

void startfunc( int zone ){
  Serial.println( String(millis()) + " Start " + String(zone) );  
}

void endfunc( int zone ){
  Serial.println( String(millis()) + " End " + String(zone) );  
}

void setup() {
    control_init();

    sensors_init();
    
    Serial.begin(9600);

    delay(2000);
  
    control_task( startfunc, endfunc, 2, 4000 );
    control_task( startfunc, endfunc, 3, 2500 );
}

void loop() {

  control_run();

  delay(100);

}
