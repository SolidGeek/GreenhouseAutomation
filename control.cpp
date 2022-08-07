#include "control.h"

uint32_t control_timer;

// bool top_window_state = false; // False = open, true = closed

// Tasks for async control
typedef struct{
    uint8_t id;
    uint8_t attached;  
    uint16_t runtime; 
    uint32_t timer;
    uint8_t zone;               
    void *start;
    void *end;
} task;

static task tasks[MAX_TASKS];

// Control setpoints for automatic mode 
static float ref_temp[3] = {20,20,20};
static float ref_hum[3] = {50,50,50};
static float temp_margin = 1.0;
static float control_margin = 500; // In milliseconds (changes smaller than this is just stupido)


void control_init()
{
    // Default mode is auto, but for now manuel
    system_state = MODE_AUTO;

    pinMode( pin_change_mode, INPUT_PULLUP );
  
    for (int i = 0; i < zones_num; i++) {
        // Configurations of pins for manual control
        pinMode(pin_open[i], INPUT_PULLUP);
        pinMode(pin_close[i], INPUT_PULLUP);

        // Configuration of pins used to open and close side-windows
        pinMode(pout_open[i], OUTPUT);
        pinMode(pout_close[i], OUTPUT);

        // Stop the motors as soon as they are configured
        digitalWrite(pout_open[i], LOW);
        digitalWrite(pout_close[i], HIGH);

        // Configuration of pins used to open and close top-windows
        pinMode(pout_kip_open[i], OUTPUT);
        pinMode(pout_kip_close[i], OUTPUT);

         // Stop the motors as soon as they are configured
        digitalWrite(pout_kip_open[i], LOW);
        digitalWrite(pout_kip_close[i], HIGH);
    }
}

void control_run(){

    system_state = MODE_AUTO; 

    if( digitalRead(pin_change_mode) ){
      system_state = MODE_MANUEL;
    }

    if( system_state == MODE_AUTO ){
        // Run P controller once every 5 min
        if( millis() - control_timer > CONTROL_INTERVAL_MS){
            control_auto();
            // Reset timer, ready for next
            control_timer = millis();
        }
            
    }

    if( sensor_data.rain_state == true && top_window_state == false ){
      for (int i = 0; i < zones_num; i++) {
        // Close all top windows for 35s 
        control_task( control_close_top, control_stop, i, 35000 );
      }
      top_window_state = true; // All top windows are now closed
    }

    // Always listen for manuel control commands
    control_manuel();
    
    control_manager();

}

void control_auto(){

    float error;
    float delta;
    float control;
    int dir;

    // Automation is some simple P control and logic
    // Regulation done for each zone (they are kept seperate by doors)
    for (size_t i = 0; i < zones_num; i++)
    {
        if ( abs( ref_temp[i] - sensor_data.temp_inside[i] ) > temp_margin )
        {
            // Regulate after temperature 
            error = ref_temp[i] - sensor_data.temp_inside[i];

            // Delta is used to determine how the error is corrected
            delta = sensor_data.temp_outside - sensor_data.temp_inside[i];   
            if( delta > 0) dir = 1; else dir = -1;

            control = temp_gain * error * (float)dir; 
            Serial.print("Zone " + (String)i + " - Temperature error: " + (String)error + ", output:"); Serial.println(abs(control));
        }else{
            // Regulate after humidity 
            error = ref_hum[i] - sensor_data.hum_inside[i];

            // The humidity will always be lower outside than inside, thus, no need for delta calculation for humidity.

            /* delta = hum_outside - hum_inside[i];
               if( delta > 0) dir = 1; else dir = -1; */

            control = hum_gain * error;  // * (float)dir; 
            Serial.print("Zone " + (String)i + " - Humidty error: " + (String)error + ", output:"); Serial.println(abs(control));
        }

        // Depending on the sign of the control signal, either open or close windows.
        
        if( abs( control ) > control_margin ){
            
            if( control > 0){
                // Open windows in zone i, for a total of "control" milliseconds
                control_task( control_open, control_stop, i, abs(control) );
            }else{
                // Close windows in zone i, for a total of "control" milliseconds
                control_task( control_close, control_stop, i, abs(control) );
            }
        }
    }
}

void control_manuel(){

    static bool manuel_change[3] = {false};

    for (int i = 0; i < zones_num; i++) {
        if(digitalRead(pin_open[i]) == LOW){
            control_open(i);
            manuel_change[i] = true;
        }
        else if(digitalRead(pin_close[i]) == LOW){
            control_close(i);  
            manuel_change[i] = true;
        }else{
          // If manuel is no longer used, stop motors
          if( manuel_change[i] ){
            control_stop(i);
            control_timer = millis();
            manuel_change[i] = false;
          }
        }
        
    }

}

void control_task( void *start, void *end, uint8_t zone, uint16_t runtime  ){
    int i = 0;
    while(i < MAX_TASKS)
    {
        // Check if there is a free spot in the task manager
        if(tasks[i].attached != 1)
        {
            tasks[i].id = i;

            tasks[i].start = start;
            tasks[i].end = end;
            tasks[i].attached = 1;
            tasks[i].zone = zone;
            tasks[i].runtime = runtime;
            tasks[i].timer = 0;

            break;
        }
        i++;
    }
}

void control_manager(){

    void (*p)(int);
  
    for(uint8_t i = 0; i < MAX_TASKS; i++)
    {
        if(tasks[i].attached == 1)
        {
            // Pointer to task
            task t = tasks[i];

            // Start the task
            if( t.timer == 0 ){
                tasks[i].timer = millis();

                p = (void *)t.start;
                (*p)(t.zone);
            }
            // End the task
            else if(  millis() - t.timer > t.runtime ){
                tasks[i].attached = 0;
   
                p = (void *)t.end;
                (*p)(t.zone);
            }
        }
    }
}

void control_stop( uint8_t zone )
{
    digitalWrite(pout_kip_open[zone], LOW);
    digitalWrite(pout_kip_close[zone], HIGH);

    digitalWrite(pout_open[zone], LOW);
    digitalWrite(pout_close[zone], HIGH);
}


void control_open( uint8_t zone)
{
    digitalWrite(pout_open[zone], LOW);
    digitalWrite(pout_close[zone], LOW);

    // Only open the top windows if it is not raining
    if(sensor_data.rain_state == false){
        top_window_state = false;
        digitalWrite(pout_kip_open[zone], LOW);
        digitalWrite(pout_kip_close[zone], LOW);
    }
}

void control_close( uint8_t zone)
{
    digitalWrite(pout_open[zone], HIGH);
    digitalWrite(pout_close[zone], HIGH);

    control_close_top( zone );
}

void control_close_top( uint8_t zone){
    // Close the top windows 
    digitalWrite(pout_kip_open[zone], HIGH);
    digitalWrite(pout_kip_close[zone], HIGH);
}
