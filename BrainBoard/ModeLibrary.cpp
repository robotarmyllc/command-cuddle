/**
 * Mode Library :: Convenience routines to animate the robot.
 *
 */
#include "ModeLibrary.h"
#include "BrainBoard.h"

ModeLibrary::ModeLibrary() { }

/**
  * Reset motor and LED values.
  */
void ModeLibrary::begin() {
   servoA = servoB = servoC = 0.0;
   minR = ledR = 0;
   minG = ledG = 0;
   minB = ledB = 0;
   maxR = 255;
   maxG = 255;
   maxB = 255;
   incR = ledRDir = 2;  //  -1 = fade LED,   1 = increase LED each cycle.
   incG = ledGDir = 2;  //  -1 = fade LED,   1 = increase LED each cycle.
   incB = ledBDir = 2;  //  -1 = fade LED,   1 = increase LED each cycle.
}

/**
 *  Configure the LED settings.
 *  Leave the min and max alone but change how the colors ramp.
 */
void ModeLibrary::configLED( int inR, int inG, int inB   /*  Usually 1 or 2 */ ) {
  configLED( 0,0,0,  255,255,255, inR,inG,inB );
}

/**
 *  Configure the LED settings.  
 *  You usually don't need to call this if you want just white.
 */
void ModeLibrary::configLED( 
          int mnR, int mnG, int mnB,  // Usually zero
          int mxR, int mxG, int mxB,  // No more than 255
          int inR, int inG, int inB   //  Usually 1 or 2
  ) {
  minR = ledR = mnR;
  minG = ledG = mnG;
  minB = ledB = mnB;

  maxR = mxR;
  maxG = mxG;
  maxB = mxB;

  incR = ledRDir = inR;
  incG = ledGDir = inG;  
  incB = ledBDir = inB;
}
          
void ModeLibrary::fadeLED() {
  ledR+=ledRDir;
  ledG+=ledGDir;
  ledB+=ledBDir;
  
  if ( ledR < minR ) {
    ledR = 0;
    ledRDir = -ledRDir;
  }
  if ( ledG < minG ) {
    ledG = 0;
    ledGDir = -ledGDir;
  }
  if ( ledB < minB ) {
    ledB = 0;
    ledBDir = -ledBDir;
  }
  if ( ledR > maxR ) {
    ledR = maxR;
    ledRDir = -ledRDir;
  }
  if ( ledG > maxG ) {
    ledG = maxG;
    ledGDir = -ledGDir;
  }
  if ( ledB > maxB ) {
    ledB = maxB;
    ledBDir = -ledBDir;
  }
  brain.setLED(ledR,ledG,ledB); 
}

void ModeLibrary::pause(int mn, int ms) {
  while( mn > 0 ) {
    fadeLED();
    mn -= 5;
    delay(5);
  }
  ms -= mn;
  while( ms > 0 ) {
    fadeLED();
    if ( ms <= 50 ) {
      delay(ms);
      ms = 0;
    } else {
      int d = rand()%50;
      ms -= d;
      delay(d);
    }
  }
}

/**
 * Return random value between 0 and 1
 */
int ModeLibrary::coinFlip(void) {
  return rand() % 2;
}

/**
 *  Jab in some direction and return back
 */
void ModeLibrary::jab() {
  // Remember our starting position.
  double svA = servoA;
  double svB = servoB;
  double svC = servoC;
  
  // Pick a random direction to jab in.
  switch( rand()%6 ) {
    case 0:
      servoC =- 0.5;
      updateServos();
      robotRamp(2, 1.0, 1.0, -1.0 );
      break;
    case 1:
      servoB =- 0.5;
      updateServos();
      robotRamp(2, 1.0, -1.0, 1.0 );
      break;
    case 2:
      servoB =- 0.5;
      servoC =- 0.5;
      updateServos();
      robotRamp(2, 1.0, -1.0, -1.0 );
      break;
    case 3:
      servoA =- 0.5;
      servoC =- 0.5;
      updateServos();
      robotRamp(2, -1.0, 1.0, -1.0 );
      break;
    case 4:
      servoA =- 0.5;
      servoB =- 0.5;
      updateServos();
      robotRamp(2, -1.0, -1.0,  1.0 );
      break;
    case 5:
      servoA =- 0.5;
      updateServos();
      robotRamp(2, -1.0, 1.0,  1.0 );
      break;
  }
  pause(100, 300);
  
  // Sometimes recoil from jab
  if ( coinFlip() ) {
    robotRamp( 6, svA, svB, svC );
    pause(1, 300);
  }
}

/**
 *  Twitch or shudder.
 */
void ModeLibrary::twitch() {
  // Pick random motors to twitch
  double twitchValA = 0.0;
  double twitchValB = 0.0;
  double twitchValC = 0.0;
  
  if ( coinFlip() ) {
    twitchValA = 0.2;
  }
  if ( coinFlip() ) {
    twitchValB = 0.2;
  }
  if ( coinFlip() ) {
    twitchValC = 0.2;
  }
  
  for ( int i=0; i<rand()%20+10; i++ ) {
    robotDo(  
       servoA + twitchValA, 
       servoB + twitchValB,
       servoC + twitchValC,  
       ledR, ledG, ledB );
    robotDo(  
       servoA, 
       servoB, 
       servoC,  
       ledR, ledG, ledB );
  }
}

/**
 * Dip straight down then return back.
 *  nBobs ::  Number of times to bob
 *   pmn1 ::  first pause minimum in milliseconds (mS)
 *   pmx1 ::  first pause maximum in milliseconds (mS)
 *   pmn2 ::  first pause minimum in milliseconds (mS)
 *   pmx2 ::  first pause maximum in milliseconds (mS)
 */
void ModeLibrary::bob( int nBobs, int pmn1, int pmx1, int pmn2, int pmx2 ) {
  
  double svA = servoA;
  double svB = servoB;
  double svC = servoC;
  
  for ( int i=0; i< nBobs; i++) {
    servoA -= 0.2;
    servoB -= 0.2;
    servoC -= 0.2;   
    updateServos();
    //robotDo(servoA, servoB, servoC, ledR, ledG, ledB );
  }
  pause(pmn1, pmx1);
  servoA = svA;
  servoB = svB;
  servoC = svC;
  
  updateServos();
  //robotDo(servoA, servoB, servoC, ledR, ledG, ledB );
  pause(pmn2, pmx2);
}

/**
 * Descend straight down slow.
 */
void ModeLibrary::mope() {
  robotRamp( 40, -0.7, -0.7, -0.7 );
  robotRamp( 40, -1.0, -1.0, -1.0 );
  robotRamp( 10,  -0.9, -0.9, -0.9 );  // This does not work.  It should bounce off the bottom a little.
}

/**
 * Rise up  slow.
 */
void ModeLibrary::perk() {
  robotRamp( 200, (dRandom()*0.15)+0.15, (dRandom()*0.15)+0.15, (dRandom()*0.15)+0.15 );
  robotRamp( 100, (dRandom()*0.15)+0.15, (dRandom()*0.15)+0.15, (dRandom()*0.15)+0.15 );
  robotRamp(  50, (dRandom()*0.15)+0.15, (dRandom()*0.15)+0.15, (dRandom()*0.15)+0.15 );
  pause(500,700);
  robotRamp( 100, -0.1, -0.1, -0.1 );
}

void ModeLibrary::swirl( int rate ) {
  double frame = 0.0;
  
  robotRamp( 40, sin(frame/100.0), sin((frame+100.0)/100.0), sin((frame+200.0)/100.0) );
  while (frame < 628) {
    servoA = sin(frame/100.0);
    servoB = sin((frame+100.0)/100.0);
    servoC = sin((frame+200.0)/100.0);
    frame+=(double)rate;
   
   updateServos();
  }
}

void ModeLibrary::robotRamp( int steps, double sa, double sb, double sc ) {
  double ra = (servoA - sa)/steps;
  double rb = (servoB - sb)/steps;
  double rc = (servoC - sc)/steps;
   
  if ( steps < 5 ) {
    for ( int i=0; i< steps; i++ ) {
      servoA-=ra;
      servoB-=rb;
      servoC-=rc;
      updateServos();
      delay(30);  // This can be a little too fast.
    }  
  } else {
    for ( int i=0; i< steps/5; i++ ) {
      servoA-=ra;
      servoB-=rb;
      servoC-=rc;
      updateServos();
    }  
    for ( int i=0; i< steps/5; i++ ) {
      servoA-=ra*3.0;
      servoB-=rb*3.0;
      servoC-=rc*3.0;
      updateServos();
    }  

    for ( int i=0; i< steps/5; i++ ) {
      servoA-=ra;
      servoB-=rb;
      servoC-=rc;
      updateServos();
    }
  }
}

void ModeLibrary::rangeCheckServos() {
  if ( servoA < -1.0 )  servoA = -1.0; 
  if ( servoB < -1.0 )  servoB = -1.0; 
  if ( servoC < -1.0 )  servoC = -1.0;
  
  if ( servoA > 1.0 )  servoA = 1.0; 
  if ( servoB > 1.0 )  servoB = 1.0; 
  if ( servoC > 1.0 )  servoC = 1.0; 
}

void ModeLibrary::updateServos() {
  rangeCheckServos();
  robotDo( servoA, servoB, servoC, ledR, ledG, ledB );
}

/**
 * Set the motors and LED to the supplied values.
 */
void ModeLibrary::robotDo(double sa, double sb, double sc, byte r, byte g, byte b) {
  fadeLED();
  servos.updateServos( getServoVal(sa), getServoVal(sb), getServoVal(sc) );
  brain.setLED(r,g,b);  
  delay(18); 
}

/**
 * in = -1.0 to 1.0
 out = 80-180
 */
int ModeLibrary::getServoVal( double in ) {
  int val = (255*((in+1.0)/2.0));

  if ( val < 0 ) {
    val = 0;
  }
  
  if ( val > 255 ) {
    val = 255;
  }
  
  return val;
}

/**
 * Return a random double value between -1.0 and 1.0
 */
double ModeLibrary::dRandom() {
  return ((rand() % 2000)/1000.0) - 1.0;
}

/**
 * Go to a random location in over a quantity of steps.
 */
void ModeLibrary::robotRandom(int steps) {
  robotRamp( steps, dRandom(), dRandom(), dRandom() );
}


