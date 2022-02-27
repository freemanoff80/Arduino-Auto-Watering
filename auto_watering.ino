#include <EncButton.h>
//#include <EncButton2.h>
#define _EB_HOLD 3000

#include <LiquidCrystal_I2C.h>


EncButton<EB_TICK, 8, 9, 10> enc;  // энкодер с кнопкой <A, B, KEY>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define HUMIDITY_SENSOR A6
//int HUMIDITY_SENSOR = A6;
int PUMP = 2;
int LED_PUMP = 3;
int LED_ERROR_STATUS = 4;

boolean switcher = 1;

// ---------- Timers
unsigned long TIMER_TEST;
unsigned long TIMER_COUNT_VALUES;

// ---------- Humidity Level Sensor
unsigned long HUMIDITY_SENSOR_TIMER_CHECK = 0;
unsigned long HUMIDITY_SENSOR_VALUE;
boolean LOW_HUMIDITY_STATUS = 0;
boolean LOW_HUMIDITY_STATUS_FLAG = true;
boolean LOW_HUMIDITY_STATUS_TIMER_FLAG = true;
unsigned long LOW_HUMIDITY_STATUS_TIMER;

// ---------- Pump
unsigned long PUMP_SWITCH_ON_TIMER;
unsigned long PUMP_SWITCH_OFF_TIMER;
int PUMP_SWITCH_STATUS = 0;
int PUMP_SWITCH_FLAG = 0;
boolean PUMP_INCLUDE = true;

// ---------- Liquid Level Sensor
int LIQUID_LEVEL_SENSOR = 11;
boolean LIQUID_LEVEL_STATUS;
unsigned long LIQUID_LEVEL_STATUS_TIMER = 0;
boolean LIQUID_LEVEL_STATUS_FLAG = true;
boolean LIQUID_LEVEL_STATUS_TIMER_FLAG = true;

// ---------- Encoder Button
unsigned long ENCODER_BUTTON_HOLD_TIMER;
boolean ENCODER_BUTTON_HOLD_FLAG = 0;
boolean ENCODER_BUTTON_HOLD_STATUS = 0;


// ---------- Monitor Values
unsigned long WATERING_TIMER_COUNT;
unsigned long ERRORS_VAL = 0;
unsigned long WATERING_INTERVAL_RECOMENDED = 0;
unsigned long TEST_VAL = 555;

char MONITOR_WATERING_TIMER[20];
char MONITOR_HUMIDITY_SENSOR_VALUE[20];
char MONITOR_ERRORS[20];
char MONITOR_WATERING_INTERVAL_RECOMENDED[20];
char MONITOR_TEST_VAL[20];


// ---------- Settings Values
boolean SETTINGS_MODE_STATUS = false;
unsigned long SETTINGS_MODE_TIMER_COUNT;
int SETTINGS_CHANGE_INCREMENT = 0;

int WATERING_TIMER = 2;
int HUMIDITY_LIMIT_MIN = 20;
int PUMPING_TIME = 5;
int LEARNING = 0;
int FORCE_WATERING = 0;
int SETTINGS_MODE_TIMER = 30;
int LCD_BACKLIGHT_TIMER = 40;
int ERRORS_DETECTION_TIME = 1;

char OUTPUT_WATERING_TIMER[20];
char OUTPUT_HUMIDITY_LIMIT_MIN[20];
char OUTPUT_PUMPING_TIME[20];
char OUTPUT_LEARNING[20];
char OUTPUT_FORCE_WATERING[20];
char OUTPUT_SETTINGS_MODE_TIMER[20];
char OUTPUT_LCD_BACKLIGHT_TIMER[20];
char OUTPUT_ERRORS_DETECTION_TIME[20];


// ---------- Errors Values
int COUNT_ERROR = 0;
int ERRORS_TOTAL = 0;
boolean ERROR_STATUS = false;
boolean ERROR_STATUS_FLAG;

// ---------- Display Vars
int CURSOR_CURRENT_LINE = 0;
int CURSOR_CURRENT_LINE_MEMORY = 0;
int CURSOR_SIZE_ARRAY;
int SCREEN_NUMBER;
int DISPLAY_LINES = 4;
unsigned long DISPLAY_UPDATE_TIMER;
boolean DISPLAY_UPDATE_FORCE;
boolean DISPLAY_CURSOR_VISIBLE = false;
boolean MONITOR_MODE_STATUS = true;

// ---------- LCD BackLight
boolean LCD_BACKLIGHT_STATUS = 1;
unsigned long LCD_BACKLIGHT_TIMER_COUNT;


//boolean LEARNING = true;
//int HUMIDITY_LIMIT_MAX = 70;

  // ---------- Monitoring Arrays
  const char *monitoring_Names_ARRAY[] = {
  "WaterTimer",
  "Humidity",  
  "Errors",
  "WaterIntRec",
  "TestValue-1",
  };

  const int SIZE_monitoring_ARRAY = sizeof( monitoring_Names_ARRAY )/sizeof( int );

  char *monitoring_Values_ARRAY[ SIZE_monitoring_ARRAY ] = {
  MONITOR_WATERING_TIMER,
  MONITOR_HUMIDITY_SENSOR_VALUE,
  MONITOR_ERRORS,
  MONITOR_WATERING_INTERVAL_RECOMENDED,
  MONITOR_TEST_VAL,
  };
  

  // ---------- Settings Arrays
  const char *settings_Names_ARREA[] = {
    "WaterTimer",
    "HumMinLim",  
    "PumpTime",
    "Lerning",
    "ForceWater",
    "SetModTime",
    //"LCDBLTime",
    "BckLghtTime",
    "ErrDetTime",
  };

  const int SIZE_settings_ARREA = sizeof( settings_Names_ARREA )/sizeof( int );

  int *settings_Values_ARREA[3][ SIZE_settings_ARREA ] = {
  { // Default Settings Values
    &WATERING_TIMER,
    &HUMIDITY_LIMIT_MIN,
    &PUMPING_TIME,
    &LEARNING,
    &FORCE_WATERING,
    &SETTINGS_MODE_TIMER,
    &LCD_BACKLIGHT_TIMER,
    &ERRORS_DETECTION_TIME,
  },
  { // Low Limit Settings Values
    1,
    0,
    1,
    0,
    0,
    10,
    10,
    0,
  },
  { // High Limit Settings Values
    336,
    90,
    10,
    1,
    1,
    120,
    120,
    60,
  }
  };


  char *settings_Output_ARREA[ SIZE_settings_ARREA ] = {
    OUTPUT_WATERING_TIMER,
    OUTPUT_HUMIDITY_LIMIT_MIN,
    OUTPUT_PUMPING_TIME,
    OUTPUT_LEARNING,
    OUTPUT_FORCE_WATERING,
    OUTPUT_SETTINGS_MODE_TIMER,
    OUTPUT_LCD_BACKLIGHT_TIMER,
    OUTPUT_ERRORS_DETECTION_TIME,
  };

  // ---------- Errors Arrays
  const char *errors_Names_ARRAY[] {
    "None",
    "LowHum",
    "NotWat",
  };

  const int SIZE_errors_ARRAY = sizeof( errors_Names_ARRAY )/sizeof( int );

  int errors_Count_ARRAY[ SIZE_errors_ARRAY ];

/*
  int TEST_VALUE_1 = 25;
  int TEST_VALUE_2 = 50;
  int TEST_VALUE_3 = 75;

  int tVal = 10;
  int *tPtr = &tVal;
  //int *tPtr;
  //tPtr = &tVal;

  // ---------- Test Array
  int *test_ARREA[3] = {
    &TEST_VALUE_1,
    &TEST_VALUE_2,
    &TEST_VALUE_3,
  };
*/

// ================== Function For Output On Display

void printDisplay(  int DISPLAY_LINES,
                    int CURSOR_CURRENT_LINE,
                    int SIZE_ARREA,
                    const char **ARREA_NAMES,
                    //unsigned long **ARREA_VALUES,
                    char **ARREA_VALUES,
                    boolean CURSOR_VISIBLE = true ) {
             
    int SCREEN_NUMBER;
    
    lcd.clear();
    SCREEN_NUMBER = CURSOR_CURRENT_LINE / DISPLAY_LINES;
    
    for ( int LINE = 0; LINE < DISPLAY_LINES; LINE ++) {
  
      lcd.setCursor( 0, LINE );
      
      if ( CURSOR_CURRENT_LINE == SCREEN_NUMBER * DISPLAY_LINES + LINE ) {
        if ( CURSOR_VISIBLE ) lcd.print( ">" );
        else lcd.print( " " );
      }
      else lcd.print( " " );

      if ( SCREEN_NUMBER * DISPLAY_LINES + LINE == SIZE_ARREA ) break;

      lcd.setCursor( 1, LINE );
      lcd.print( ARREA_NAMES[ SCREEN_NUMBER * DISPLAY_LINES + LINE ] );

      lcd.setCursor( 13, LINE );
      lcd.print( ARREA_VALUES[ SCREEN_NUMBER * DISPLAY_LINES + LINE ] );
    }  
}
// ====================================================================


// ================== Function To Define Cursor Position
int cursorPosition( 
                    int CURSOR_CURRENT_POSITION,
                    int TOTAL_ARREA_SIZE,
                    boolean MONITOR_MODE = true ) {

  if ( enc.right() ) {

    //Serial.println( "Test Inside Left" );
    if ( MONITOR_MODE ) {
      CURSOR_CURRENT_POSITION += 4;
      if ( CURSOR_CURRENT_POSITION > TOTAL_ARREA_SIZE - 1) CURSOR_CURRENT_POSITION -= 4;
    }
    else {
      CURSOR_CURRENT_POSITION ++;
      if ( CURSOR_CURRENT_POSITION > TOTAL_ARREA_SIZE - 1) CURSOR_CURRENT_POSITION --;
    }
  }

  if ( enc.left() ) {
    //Serial.println( "Test Inside Right" );
    if ( MONITOR_MODE ) {
      CURSOR_CURRENT_POSITION -= 4;
      if ( CURSOR_CURRENT_POSITION < 0 ) CURSOR_CURRENT_POSITION = 0;
    }
    else {
      CURSOR_CURRENT_POSITION --;
      if ( CURSOR_CURRENT_POSITION < 0 ) CURSOR_CURRENT_POSITION = 0;
    }
  }

  return CURSOR_CURRENT_POSITION;
}
// ====================================================================


// ================== Function For Automatic Learning Pump Switch Delay

unsigned long learn_pump_delay( int HUMIDITY_CURRENT, int HUMIDITY_LIMIT, unsigned long PUMP_DELAY_CURRENT ) {
    unsigned long PUMP_DELAY_RECOMEND = ( ( PUMP_DELAY_CURRENT / ( 100 - HUMIDITY_CURRENT ) ) * ( 100 - HUMIDITY_LIMIT ) );
    
    if ( PUMP_DELAY_RECOMEND >= ( 60000 * 60) * 96  ) {
      PUMP_DELAY_RECOMEND = ( 60000 * 60) * 96 ;
    }

    if ( PUMP_DELAY_RECOMEND <= ( 60000 * 60) * 1 ) {
      PUMP_DELAY_RECOMEND = ( 60000 * 60) * 1 ;
    }
    
    return PUMP_DELAY_RECOMEND;
}

// ====================================================================


void setup() {

  Serial.begin(9600);
  pinMode( PUMP, OUTPUT);   // PUMP_SWITCH
  pinMode( LED_PUMP, OUTPUT);   // PUMP_LED
  pinMode( LED_ERROR_STATUS, OUTPUT);   // LED_HUMIDITY_STATUS
  pinMode( LIQUID_LEVEL_SENSOR, INPUT_PULLUP ); // Liquid Level Sensor
  //pinMode( HUMIDITY_SENSOR, INPUT);   // HUMIDITY_SENSOR
  delay ( 10000 );    // Delay For Calibration Humidity Sensor

  LCD_BACKLIGHT_TIMER_COUNT = millis();

  CURSOR_SIZE_ARRAY = SIZE_monitoring_ARRAY;
  strcpy( OUTPUT_FORCE_WATERING, "Off" );

  // Settins For LCD
  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку  

  //errors_Count_ARRAY[1] = 1;
  //errors_Count_ARRAY[2] = 1;

}


void loop() {

  if ( switcher == 1 ) {

    enc.tick(); // Reads Encoder Value

    LIQUID_LEVEL_STATUS = digitalRead( LIQUID_LEVEL_SENSOR ); // Read Signal From Liquid Level Sensor 
    SETTINGS_CHANGE_INCREMENT = 0;


    // ================== Encoder Button Hold
    if ( enc.hold() && ENCODER_BUTTON_HOLD_FLAG == 0 || !MONITOR_MODE_STATUS && millis() - SETTINGS_MODE_TIMER_COUNT > SETTINGS_MODE_TIMER * 1000L ) {
      ENCODER_BUTTON_HOLD_FLAG = 1;
      MONITOR_MODE_STATUS = !MONITOR_MODE_STATUS;   // Change Status Monitor Output Mode On Display
      if ( MONITOR_MODE_STATUS ) {
        SETTINGS_MODE_STATUS = 0;
        CURSOR_SIZE_ARRAY = SIZE_monitoring_ARRAY;
      }
      else {
        SETTINGS_MODE_TIMER_COUNT = millis();   // For Exit From Settings Mode To Monitor Mode By Timer Ended
        CURSOR_SIZE_ARRAY = SIZE_settings_ARREA;
      }
      DISPLAY_CURSOR_VISIBLE = !DISPLAY_CURSOR_VISIBLE;
      CURSOR_CURRENT_LINE = 0;
    }
    if ( !enc.hold() && ENCODER_BUTTON_HOLD_FLAG == 1 ) {
      ENCODER_BUTTON_HOLD_FLAG = 0;
    }


    // ================== Encoder Button Press For On or Off Settings Change Mode
    if ( !MONITOR_MODE_STATUS && enc.click() ) {
      SETTINGS_MODE_STATUS = !SETTINGS_MODE_STATUS;
      //Serial.println( SETTINGS_MODE_STATUS );
    }


    // ==================== LCD BACKLIGHT On/Off And Change Settings

    // ================== LCD BackLight On By Pressing Or Turning Encoder Handle
    if ( enc.turn() || enc.press() || enc.turnH()) {      

      SETTINGS_MODE_TIMER_COUNT = millis();   // For Exit From Settings Mode To Monitor Mode By Timer Ended
      LCD_BACKLIGHT_TIMER_COUNT = millis();

      LCD_BACKLIGHT_STATUS = 1;
      
      lcd.backlight();

      if ( !SETTINGS_MODE_STATUS ) {  
        CURSOR_CURRENT_LINE = cursorPosition( 
                    CURSOR_CURRENT_LINE,
                    CURSOR_SIZE_ARRAY,
                    //SIZE_monitoring_ARRAY,
                    MONITOR_MODE_STATUS
                    );
      }

      if ( SETTINGS_MODE_STATUS ) {   

        if (enc.left()) SETTINGS_CHANGE_INCREMENT = 1;
        if (enc.right()) SETTINGS_CHANGE_INCREMENT = -1;

        if (enc.leftH()) SETTINGS_CHANGE_INCREMENT = 10;
        if (enc.rightH()) SETTINGS_CHANGE_INCREMENT = -10;

        *settings_Values_ARREA[0][ CURSOR_CURRENT_LINE ] = *settings_Values_ARREA[0][ CURSOR_CURRENT_LINE ] + SETTINGS_CHANGE_INCREMENT;
        
        if ( *settings_Values_ARREA[0][ CURSOR_CURRENT_LINE ] < (int)settings_Values_ARREA[1][ CURSOR_CURRENT_LINE ] ) {
          *settings_Values_ARREA[0][ CURSOR_CURRENT_LINE ] = (int)settings_Values_ARREA[1][ CURSOR_CURRENT_LINE ];
        };
        
        if ( *settings_Values_ARREA[0][ CURSOR_CURRENT_LINE ] > (int)settings_Values_ARREA[2][ CURSOR_CURRENT_LINE ] ) {
          *settings_Values_ARREA[0][ CURSOR_CURRENT_LINE ] = (int)settings_Values_ARREA[2][ CURSOR_CURRENT_LINE ];
        };          
      }
    }

    // ================== LCD BackLight Off By Timer
    if ( LCD_BACKLIGHT_STATUS && millis() - LCD_BACKLIGHT_TIMER_COUNT > LCD_BACKLIGHT_TIMER * 1000L ) {

      LCD_BACKLIGHT_STATUS = 0;
      lcd.noBacklight();

      //Serial.println( LCD_BACKLIGHT_STATUS );
    }

    
    // ==================== Get Data From Humidity Sensor
    if ( millis() - HUMIDITY_SENSOR_TIMER_CHECK > 10000 ) {    // Check Humidity Sensor Value With Interval = 10 sec      

      HUMIDITY_SENSOR_VALUE = analogRead( HUMIDITY_SENSOR );
      HUMIDITY_SENSOR_VALUE = map( analogRead( HUMIDITY_SENSOR ), 452, 235, 0, 100);
      constrain( HUMIDITY_SENSOR_VALUE, 0, 100 );

      Serial.print( HUMIDITY_SENSOR_VALUE );
      Serial.println( " - Humidity" );

      HUMIDITY_SENSOR_TIMER_CHECK = millis();  // Reset Timer Humidiry Sensor Check
      
    }


    // ================== Check Low Humidity For Error
    if ( HUMIDITY_SENSOR_VALUE <= HUMIDITY_LIMIT_MIN ) {      
      
      LOW_HUMIDITY_STATUS = true;
    }
    else {
      LOW_HUMIDITY_STATUS = false;
    }

    if ( LOW_HUMIDITY_STATUS && LOW_HUMIDITY_STATUS_FLAG ) {

      if ( LOW_HUMIDITY_STATUS_TIMER_FLAG ) {
        LOW_HUMIDITY_STATUS_TIMER = millis();
        LOW_HUMIDITY_STATUS_TIMER_FLAG = false;
      }

      if ( millis() - LOW_HUMIDITY_STATUS_TIMER > 60000 * ERRORS_DETECTION_TIME ) {
        
        LOW_HUMIDITY_STATUS_FLAG = 0;
        errors_Count_ARRAY[1] = 1;  // Add ID Error To Error Array
        }
      }

    if ( !LOW_HUMIDITY_STATUS ) {

      if ( !LOW_HUMIDITY_STATUS_TIMER_FLAG ) {
        LOW_HUMIDITY_STATUS_TIMER_FLAG = true;
      }

      if ( !LOW_HUMIDITY_STATUS_FLAG ) {
        LOW_HUMIDITY_STATUS_FLAG = 1;
        errors_Count_ARRAY[1] = 0;  // Delete ID Error From Error Array
      }
    }


    // ================== Check Low Liquid Level For Error
    if ( LIQUID_LEVEL_STATUS && LIQUID_LEVEL_STATUS_FLAG ) {

      if ( LIQUID_LEVEL_STATUS_TIMER_FLAG ) {
        LIQUID_LEVEL_STATUS_TIMER = millis(); // Reset Low Liquid Level Timer
        LIQUID_LEVEL_STATUS_TIMER_FLAG = false;
        //Serial.println( "### Liquid Level Timer RESET ###" );
      }
        
      if ( millis() - LIQUID_LEVEL_STATUS_TIMER > 60000 * ERRORS_DETECTION_TIME ) {
          LIQUID_LEVEL_STATUS_FLAG = 0;
          errors_Count_ARRAY[2] = 1;  // Add ID Error For Low Liquid Level To Error Array
          //Serial.println( "Liquid Level Error - True #####" );
      }
    }

    if ( !LIQUID_LEVEL_STATUS ) {
      
      if ( !LIQUID_LEVEL_STATUS_TIMER_FLAG ) {
        LIQUID_LEVEL_STATUS_TIMER_FLAG = true;
        //Serial.println( "### LLS Timer - TRUE ###" );
      }
      
      if ( !LIQUID_LEVEL_STATUS_FLAG ) {
        LIQUID_LEVEL_STATUS_FLAG = 1;
        errors_Count_ARRAY[2] = 0;  // Delete ID Error For Low Liquid Level To Error Array
        //Serial.println( "Liquid Level Error - False #####" );
      }
    }


    if ( FORCE_WATERING == 1 || millis() - PUMP_SWITCH_ON_TIMER > ( 60000 * 60 * WATERING_TIMER ) ) {   //  Check Timer For Switch ON Pump

      PUMP_SWITCH_FLAG = 1;
      PUMP_SWITCH_STATUS = 1;
      FORCE_WATERING = 0;    //  Reset Force Watering To Off

      strcpy( OUTPUT_FORCE_WATERING, "On" );
      digitalWrite( LED_PUMP, PUMP_SWITCH_STATUS );   // LED_PUMP_SWITCH - ON
      if ( PUMP_INCLUDE ) {
        digitalWrite( PUMP, PUMP_SWITCH_STATUS );   // PUMP_SWITCH - ON
      }
      
      PUMP_SWITCH_OFF_TIMER = millis() + ( PUMPING_TIME * 1000 ) ;    // Set Time Of Pumping
      PUMP_SWITCH_ON_TIMER = millis();   // Reset Timer Switch Pump

      Serial.print( "LED_PUMP - ON, PUMP_SWITCH_STATUS - " );
      Serial.println( PUMP_SWITCH_STATUS );

    }

    if ( PUMP_SWITCH_FLAG == 1 && millis() > PUMP_SWITCH_OFF_TIMER ) {    // Check Timer For Switch OFF Pumping

      PUMP_SWITCH_FLAG = 0;
      PUMP_SWITCH_STATUS = 0;
      
      strcpy( OUTPUT_FORCE_WATERING, "Off" );
      DISPLAY_UPDATE_FORCE = 1; // Force Update Display After Change Value
      digitalWrite( LED_PUMP, PUMP_SWITCH_STATUS );   // LED_PUMP_SWITCH - OFF
      if ( PUMP_INCLUDE ) {
        digitalWrite( PUMP, PUMP_SWITCH_STATUS );   // PUMP_SWITCH - OFF
      }
      
      Serial.print( "LED_PUMP - OFF, PUMP_SWITCH_STATUS - " );
      Serial.println( PUMP_SWITCH_STATUS );

    }


    // ================== Pumping Learning

    if ( LEARNING ) {

    }

    // ================== Timer For Update Display
    if ( millis() - DISPLAY_UPDATE_TIMER >= 1000 || CURSOR_CURRENT_LINE_MEMORY != CURSOR_CURRENT_LINE || SETTINGS_CHANGE_INCREMENT != 0 || DISPLAY_UPDATE_FORCE ) {      
      DISPLAY_UPDATE_TIMER = millis();
      DISPLAY_UPDATE_FORCE = 0;
      CURSOR_CURRENT_LINE_MEMORY = CURSOR_CURRENT_LINE;

      if ( MONITOR_MODE_STATUS ) {
        printDisplay( 
              DISPLAY_LINES,
              CURSOR_CURRENT_LINE,
              SIZE_monitoring_ARRAY,
              monitoring_Names_ARRAY,
              monitoring_Values_ARRAY,
              DISPLAY_CURSOR_VISIBLE
              );
      }

      else {

        // ---------- Converting Any Value For Settings Ouput Array
        sprintf( OUTPUT_WATERING_TIMER, "%id%ih", WATERING_TIMER / 24, WATERING_TIMER % 24 );
        ltoa( HUMIDITY_LIMIT_MIN, OUTPUT_HUMIDITY_LIMIT_MIN, 10 );
        ltoa( PUMPING_TIME, OUTPUT_PUMPING_TIME, 10 );
        ltoa( LEARNING, OUTPUT_LEARNING, 10 );
        //ltoa( FORCE_WATERING, OUTPUT_FORCE_WATERING, 10 );
        //if ( FORCE_WATERING ) strcpy( OUTPUT_FORCE_WATERING, "On" );
        //else strcpy( OUTPUT_FORCE_WATERING, "Off" );
        ltoa( SETTINGS_MODE_TIMER, OUTPUT_SETTINGS_MODE_TIMER, 10 );
        ltoa( LCD_BACKLIGHT_TIMER, OUTPUT_LCD_BACKLIGHT_TIMER, 10 );
        ltoa( ERRORS_DETECTION_TIME, OUTPUT_ERRORS_DETECTION_TIME, 10 );
        // ------------------------------------------------------


        printDisplay( 
              DISPLAY_LINES,
              CURSOR_CURRENT_LINE,
              SIZE_settings_ARREA,
              settings_Names_ARREA,
              settings_Output_ARREA,
              DISPLAY_CURSOR_VISIBLE
              );        
      }
    }


    // ================== Timer For Used Values
    if (millis() - TIMER_COUNT_VALUES >= 1000) {
      TIMER_COUNT_VALUES = millis();
      
      WATERING_TIMER_COUNT = ( ( 60000 * 60 * WATERING_TIMER ) - ( millis() - PUMP_SWITCH_ON_TIMER ) ) / 1000L ;
      //Serial.print( WATERING_TIMER_COUNT );
      //Serial.println( " - TIMER_PUMP_SWITCH" );

      // ---------- Converting Any Value For Monitor Ouput Array
      //sprintf( MONITOR_WATERING_TIMER, "%ud%uh%um%us",
      sprintf( MONITOR_WATERING_TIMER, "%ud%uh%um",
      (int)( WATERING_TIMER_COUNT / ( 60 * 60 ) ) / 24,  // Pump Switch On Timer Days
      (int)( WATERING_TIMER_COUNT / ( 60 * 60 ) ) % 24,  // Pump Switch On Timer Hours
      (int)( ( WATERING_TIMER_COUNT / 60 ) % 60 )  // Pump Switch On Timer Minutes
      //(int)( WATERING_TIMER_COUNT % 60 )  // Pump Switch On Timer Seconds
      );
      Serial.print( MONITOR_WATERING_TIMER );
      Serial.println( " - Watering Timer" );
      ultoa(HUMIDITY_SENSOR_VALUE, MONITOR_HUMIDITY_SENSOR_VALUE, 10);
      ultoa(WATERING_INTERVAL_RECOMENDED, MONITOR_WATERING_INTERVAL_RECOMENDED, 10);
      ultoa(TEST_VAL, MONITOR_TEST_VAL, 10);
      // ------------------------------------------------------

      // ---------- Change Error Value For Monitor Ouput Array
      ERRORS_TOTAL = 0;
      for ( int NUM = 0; NUM < SIZE_errors_ARRAY; NUM++ ) {
        if ( errors_Count_ARRAY[ NUM ] != 0 ) {
          if ( ERRORS_TOTAL == COUNT_ERROR ) {
            strcpy( MONITOR_ERRORS, errors_Names_ARRAY[ NUM ] );
          }
          ERRORS_TOTAL ++;
        }
      }
      if ( ERRORS_TOTAL == 0 ) {
        strcpy( MONITOR_ERRORS, errors_Names_ARRAY[ 0 ] );
      }
      COUNT_ERROR++;
      if ( COUNT_ERROR >= ERRORS_TOTAL ) {
        COUNT_ERROR = 0;
      }

      // ---------- Error Led On/Off
      if ( ERRORS_TOTAL > 0 && ERROR_STATUS_FLAG ) {
        ERROR_STATUS_FLAG = false;
        digitalWrite( LED_ERROR_STATUS, 1 ); // Led Error Status - ON
      }
      if ( ERRORS_TOTAL <= 0 && !ERROR_STATUS_FLAG ) {
        ERROR_STATUS_FLAG = true;
        digitalWrite( LED_ERROR_STATUS, 0 ); // Led Error Status - OFF
      }
      // ------------------------------------------------------  
    }
    // ========================================================


    // ================== Timer For Test 
    if (millis() - TIMER_TEST >= 1000) {
      TIMER_TEST = millis();

      /*
      WATER_TIME_DAYS = ( WATERING_TIMER_COUNT / ( 60 * 60 ) ) / 24;
      WATER_TIME_HOURS = ( WATERING_TIMER_COUNT / ( 60 * 60 ) ) % 24 ;
      WATER_TIME_MINUTES = ( WATERING_TIMER_COUNT / 60 ) % 60 ;
      WATER_TIME_SECONDS = WATERING_TIMER_COUNT % 60;
      sprintf(OUTPUT_WATER_TIME_TOTAL, "%ud,%uh,%um,%us",
      //WATER_TIME_DAYS,
      //WATER_TIME_HOURS,
      //WATER_TIME_MINUTES,
      //WATER_TIME_SECONDS
      //sprintf(OUTPUT_WATER_TIME_TOTAL, "%u,%u,%u,%u",
      (int)( WATERING_TIMER_COUNT / ( 60 * 60 ) ) / 24,
      (int)( WATERING_TIMER_COUNT / ( 60 * 60 ) ) % 24,
      (int)( ( WATERING_TIMER_COUNT / 60 ) % 60 ),
      (int)( WATERING_TIMER_COUNT % 60 ) 
      );
      Serial.println( OUTPUT_WATER_TIME_TOTAL );
      //WATER_TIME_DAYS = WATERING_TIMER / 24;
      //WATER_TIME_HOURS = WATERING_TIMER % 24;
      Serial.print( WATER_TIME_DAYS );
      Serial.println( "d - Water_Time_Days" );
      Serial.print( WATER_TIME_HOURS );
      Serial.println( "h - Water_Time_Hours" );
      Serial.print( WATER_TIME_MINUTES );
      Serial.println( "m - Water_Time_Minutes" );
      Serial.print( WATER_TIME_SECONDS );
      Serial.println( "s - Water_Time_Seconds" );
      */
      //WATER_TIME_TOTAL = (String)WATER_TIME_DAYS + "d," + (String)WATER_TIME_HOURS + "h" ;
      //Serial.println( WATER_TIME_TOTAL );
      //sprintf(OUTPUT_WATER_TIME_TOTAL, "%id,%ih", WATER_TIME_DAYS, WATER_TIME_HOURS );
      //Serial.println( OUTPUT_WATER_TIME_TOTAL );
      //Serial.println( OUTPUT_WATERING_TIMER );
      //Serial.println( (String)WATER_TIME_DAYS + "d," + (String)WATER_TIME_HOURS + "h" );
      //Serial.println( (char)WATER_TIME_DAYS + "d," + (char)WATER_TIME_HOURS + "h" );
      //Serial.println( errors_Names_ARRAY[ 0 ] );
      //Serial.println( *settings_Values_ARREA[ 1 ] );
      //Serial.println( settings_Output_ARREA[ 1 ] );
      //Serial.print( SETTINGS_CHANGE_INCREMENT );
      /*
      Serial.println( " ----------------------" );
      Serial.print( LOW_HUMIDITY_STATUS_TIMER );
      Serial.println( " - Value Humidity Timer" );
      Serial.print( LOW_HUMIDITY_STATUS );
      Serial.println( " - Low Humidity Status" );
      Serial.print( LOW_HUMIDITY_STATUS_TIMER_FLAG );
      Serial.println( " - Low Humidity Status Timer Flag" );
      Serial.println( " ----------------------" );
      */
      /*
      //int tVal = 10;
      //int *PTR = &tVal;
      //tPtr = &tVal;
      Serial.print( tVal );
      Serial.println( " - Test_Val : Before" );
      Serial.print( *tPtr );
      Serial.println( " - Pointer *PTR" );
      //*tPtr ++;
      *tPtr = *tPtr + 1;
      //&tVal ++;
      Serial.print( *tPtr );
      Serial.println( " - Pointer *PTR" );      
      Serial.print( tVal );
      Serial.println( " - Test_Val : After" );
      */
      /*
      Serial.print( SETTINGS_MODE_TIMER );
      Serial.println( " - SETTINGS_MODE_TIMER : Before #####" );
      //Serial.print( *settings_Values_ARREA[0][5] );
      //Serial.println( " - Pointer *settings_Values_ARREA[0][5] : Before" );
      //*settings_Values_ARREA[0][5] = 30;
      //*settings_Values_ARREA[0][5] = *settings_Values_ARREA[0][5] + 1;
      Serial.print( *settings_Values_ARREA[0][5] );
      Serial.println( " - Pointer *settings_Values_ARREA[0][5]" );
      Serial.print( (int)settings_Values_ARREA[1][5] );
      Serial.println( " - Pointer *settings_Values_ARREA[1][5]" );
      Serial.print( (int)settings_Values_ARREA[2][5] );
      Serial.println( " - Pointer *settings_Values_ARREA[2][5]" );
      Serial.print( SETTINGS_MODE_TIMER );
      Serial.println( " - SETTINGS_MODE_TIMER : After #####" );
      Serial.print( HUMIDITY_LIMIT_MIN );
      Serial.println( " - Humidiry Limit Min #####" );
      */
      //Serial.print( TEST_VALUE_1 );
      //Serial.println( " - Test_Value_1 : Before #####" );
      //Serial.print( *test_ARREA[0] );
      //Serial.println( " - Pointer *test_ARREA[0] : Before" );
      //*test_ARREA[0] = 30;
      //*test_ARREA[0] = *test_ARREA[0] + 1;
      //Serial.print( *test_ARREA[0] );
      //Serial.println( " - Pointer *test_ARREA[0] : After" );      
      //Serial.print( (int)*test_ARREA[0] );
      //Serial.println( " - Test 1 #####" );
      //Serial.print( *test_ARREA[1] );
      //Serial.println( " - Test 2 #####" );
      //Serial.print( *test_ARREA[2] );
      //Serial.println( " - Test 3 #####" );
      //Serial.println( " - Settings Test Value From Arrea #####" );
      //Serial.print( TEST_VALUE_1 );
      //Serial.println( " - Test_Value_1 : After #####" );
      //Serial.print( CURSOR_SIZE_ARRAY );
      //Serial.println( " - Current Cursor Size Array" );
      //Serial.println( " - Icrement In Test" );
      //Serial.println( settings_Values_ARREA[0][0] );
      //Serial.println( settings_Values_ARREA[1][0] );
      //Serial.println( settings_Values_ARREA[2][0] );
      //Serial.print( LIQUID_LEVEL_STATUS );
      //Serial.print( digitalRead( LIQUID_LEVEL_SENSOR ) );
      //Serial.println( " - Liquid Level Status" );
      //if ( LIQUID_LEVEL_STATUS ) {
      //  Serial.print( ( 12000 - ( millis() - LIQUID_LEVEL_STATUS_TIMER  ) ) / 1000L );
      //  Serial.println( " - Timer LLE TRUE" );
      //}
      //Serial.print( ( millis() - LIQUID_LEVEL_STATUS_TIMER ) / 1000);
      //Serial.println( " - SecondMetr LLE TRUE" );

      if ( LCD_BACKLIGHT_STATUS ) {
        Serial.print( ( LCD_BACKLIGHT_TIMER * 1000L - ( millis() - LCD_BACKLIGHT_TIMER_COUNT ) ) / 1000L );
        Serial.println( " - LCD Backlight Timer" );
      }
    }
    // ========================================================

    
  }
}
