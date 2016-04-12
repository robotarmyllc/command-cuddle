/*  
 * Robot Army Starter Kit
 * 
 * http://robot-army.com
 * 
 *  Arduino compatible code for the Robot Army brain board.
 *
 */
 

/*
 *   DIP Modes
 
     0 =   D/D/D/D  =   Calibrate Motors and Cycle RGB LED
     1 =   D/D/D/U  =   Motors sweep up/down through full range
     2 =   D/D/U/D  =   Chicken Mode
     3 =   D/D/U/U  =   Angry Mode
     4 =   D/U/D/D  =   Happy Mode
     5 =   D/U/D/U  =   Caffeinated Mode
     6 =   D/U/U/D  =   Sad Mode
     7 =   D/U/U/U  =   ** No mode ** <== Write your own!
 */
 
#define MAINFILE

//#include <Serial.h>  // Not needed in Arduino 1.6 and above
#include <Servo.h>
#include "BrainBoard.h"

int dipSw = 0;

void setup() {

  brain.begin();
  delta.begin();
  servos.begin();
  
  servos.refresh();

  dipSw = brain.getDIP();    
  switch ( dipSw ) {
    case 0:  // Test Mode
      servos.updateServos(0,0,0 );
      delay(1000);
      servos.updateServos(255,255,255  );
      
      break;
    case 1:  // Up-Down Mode :: Slowly move all servos together up then down. LED fades white off to on.
      // Nothing to initialize for this mode.
      break;
    case 2:  // Chicken Mode ::  Act like Sarah's parrot "Mango"
    // Nothing to initialize  for this mode.
      break;
    case 3:  // Kill Mode :: Angry Delta does not like your cuddling style!!!
      mode.angryInit();
      break;
    case 4:  // Happy Mode :: Your cuddling skills have improved.  Maybe it won't kill you...
      mode.happyInit();
      break;
    case 5:  // Caffeinated Mode :: This mode carelully emulates times when humans consume to much   SQUIRREL!!!!!!
      mode.caffeinatedInit();
      break;
    case 6:  // Sad Mode ::  Coming off that caffeine high
      mode.sadInit();
      break;        
    case 7: // Unused Mode
      // Nothing to initialize for this mode.
      break;
    
      
    case 8:   // Mode Serial Direct
      servos.updateServos(200,200,200);
      brain.flashLED(dipSw, 255, 0, 255); //LED will flash blue 1 to 7 times
      delay(800 - (dipSw*100) );
      Serial.begin(57600);
      modeSerialDirect.begin();
      servos.updateServos(
        DeltaServos::FLAT_SERVO,
        DeltaServos::FLAT_SERVO,
        DeltaServos::FLAT_SERVO  );  // Calibration :: arms horizontal with table
      break;
      
    // Serial Delta Mode @ ADDR [1-7]  (and calibration mode when no serial data is received)
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      servos.updateServos(245,245,245);
      delay(500);
      brain.flashLED(dipSw-8, 0, 0, 255);  //LED will flash blue 1 to 7 times
      delay(900 - ((dipSw-8)*100) );
      servos.updateServos(
        DeltaServos::FLAT_SERVO,
        DeltaServos::FLAT_SERVO,
        DeltaServos::FLAT_SERVO  );  // Calibration :: arms horizontal with table
      Serial.begin(57600);
      modeSerialXYZ.begin();
      break;
  } 
}

void loop() {
  
  switch ( dipSw ) {
    case 0:  // Calibration
      brain.flashOBLED();
      servos.updateServos(
        DeltaServos::FLAT_SERVO,
        DeltaServos::FLAT_SERVO,
        DeltaServos::FLAT_SERVO  );  // Calibration :: arms horizontal with table
      // LED Color Rotation
      brain.flashLED(1, 255, 0,     0, 200, 0   );  // Red
      brain.flashLED(1, 0,   255,   0, 200, 0   );  // Blue
      brain.flashLED(1, 0,   0,   255, 200, 0   );  // Green
      break;
    case 1: // Up-Down Mode :: Slowly move all servos together up then down. LED fades white off to on.
      servos.upDown(4000);
      break;
    case 2: // Chicken Mode ::  Act like Sarah's parrot "Mango"
      modeChicken.update();     
      break;
    case 3:  // Kill Mode :: Angry Delta does not like your cuddling style!!!
      //modeAngry.update();
      mode.angryUpdate();
      break;
    case 4:  // Happy Mode :: Your cuddling skills have improved.  Maybe it won't kill you...
      mode.happyUpdate();
      //modeHappy.update();
      break;
    case 5:  // Caffeinated Mode :: This mode carelully emulates times when humans consume to much   SQUIRREL!!!!!!
      mode.caffeinatedUpdate();
      //modeCaffeinated.update();
      break;
    case 6:  // Sad Mode ::  Coming off that caffeine high
      mode.sadUpdate();
      //modeSad.update();
      break;        
    case 7: // Unused Mode
      // Nothing to do for this mode.
      break;

    // Serial Delta mode channels
    case 8:  // case 8 is direct serial mode.
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:   
      // Servo values are handled by serialEvent() interupt below.
      break;   
  }
} 

void serialEvent() {
  switch(dipSw) {
    case 8:
      // This mode is for setting each motor angle directly over serial (no delta math).
      modeSerialDirect.serialEvent();
      break;
    default:  // Modes 9-15
      // This mode uses the on-board delta math to turn an XYZ value into delta motor angles.
      modeSerialXYZ.serialEvent(dipSw-9);  // We also pass the DIP value which is channel number.
      break;
  }
}
