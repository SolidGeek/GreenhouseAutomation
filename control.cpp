#include "control.h"

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

    if( system_state == MODE_AUTO ){
        // Run P controller
        control_auto();
    }else{
        control_manuel();
    }


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
        if ( abs( ref_temp[i] - temp_inside[i] ) > temp_margin )
        {
            // Regulate after temperature 
            error = ref_temp[i] - temp_inside[i];

            delta = temp_outside - temp_inside[i];   
            if( delta > 0) dir = 1; else dir = -1;

            control = temp_gain * error * (float)dir; 
        }else{
            // Regulate after humidity 
            error = ref_hum[i] - hum_inside[i];

            delta = hum_outside - hum_inside[i];
            if( delta > 0) dir = 1; else dir = -1;

            control = hum_gain * error * (float)dir; 
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

    for (int i = 0; i < zones_num; i++) {
        if(digitalRead(pin_open[i]) == LOW){
            control_open(i);
        }
        else if(digitalRead(pin_close[i]) == LOW){
            control_close(i);  
        }
        else{
            control_stop(i);
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
    window_state = false;

    digitalWrite(pout_open[zone], LOW);
    digitalWrite(pout_close[zone], LOW);

    // Only open the top windows if it is not raining
    if(rain_state == false){
        digitalWrite(pout_kip_open[zone], LOW);
        digitalWrite(pout_kip_close[zone], LOW);

        top_window_state = false;
    }
}

void control_close( uint8_t zone)
{
    digitalWrite(pout_open[zone], HIGH);
    digitalWrite(pout_close[zone], HIGH);

    // Close the top windows 
    digitalWrite(pout_kip_open[zone], HIGH);
    digitalWrite(pout_kip_close[zone], HIGH);
}
