#include "display.h"

/* 
boolean changeSettingZoneFlag = false;
boolean changeSettingUpFlag = false;
boolean changeSettingDownFlag = false;
boolean changeSystemModeFlag = false;
boolean LCDclearScreen = false;
boolean LCDblinkState = false; 
unsigned long LCDpreviousBlinkMillis;

unsigned long temporaryManuelStart;

// set the LCD address to 0x27 for a 20 chars 4 line display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void display_init(){

    pinMode(controlOpenPin, INPUT_PULLUP);
    pinMode(changeSettingZonePin, INPUT_PULLUP);
    pinMode(changeSettingUpPin, INPUT_PULLUP);
    pinMode(changeSettingDownPin, INPUT_PULLUP);
    pinMode(changeSettingToChangePin, INPUT_PULLUP);
    pinMode(changeSystemModePin, INPUT_PULLUP);

    // Initialize the lcd for 20 chars 4 lines
    lcd.begin(20,4);   
    lcd.home();
    lcd.setCursor(0,0);

}

*/
