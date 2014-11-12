/**
 * ModeChicken -- Mode that animates like a chicken/bird
 *
 */
#include "ModeChicken.h"
#include "BrainBoard.h"

ModeChicken::ModeChicken() {}

void ModeChicken::update() {
   robotSwirl();
   
   robotRandom();
   robotRandom();
   robotRandom();
   robotRandom();
   robotRandom();
  
   robotChicken();
   robotChicken();
   robotChicken();
   
   robotRandom();
   robotRandom();
   robotRandom();
   robotRandom();
   robotRandom();
   
   robotBob();
   robotBob();
   robotBob();
   
   robotRandom();
   robotRandom();
   robotRandom();
   robotRandom();
   robotRandom(); 
  
}

void ModeChicken::robotSwirl() {
  frame = 0;
  
  while (frame < maxframes) {
    frame+=2;
    double a = sin(frame/100.0);
    double b = sin((frame+100)/100.0);
    double c = sin((frame+200)/100.0);
   
   robotDo( a,b,c, sin2color(a), sin2color(b), sin2color(c) );
  }
}

void ModeChicken::robotChicken() {
  frame = 0;
  
  while (frame < maxframes) {
    frame+=20;
    double a = sin(frame/100.0);
    byte val = sin2color(a);
   
   robotDo( a, a, a, val, val, val );
  }
}

void ModeChicken::robotBob() {  
   robotDo( -1.0, 0.0, 0.0, 255, 50, 50 );
   delay(400); 
   robotDo( 1.0, 1.0, 1.0, 0, 0, 0 );
   delay(200); 
   robotDo( 0.0, -1.0, -1.0, 50, 255, 255 );
   delay(400); 
   robotDo( 1.0, 1.0, 1.0, 0, 0, 0 );
   delay(200); 
}

void ModeChicken::robotRandom() {
  robotRamp(random(20,150), dRandom(), dRandom(), dRandom(), dRandom(), dRandom(), dRandom() );
}

void ModeChicken::robotRamp( int steps, double a1, double b1, double c1, double a2, double b2, double c2 ) {
  int i = 0;
  double ra = (a1 - a2)/steps;
  double rb = (b1 - b2)/steps;
  double rc = (c1 - c2)/steps;
 
   double a = a1;
   double b = b1;
   double c = c1;
  
  for ( i=0; i< steps; i++ ) {
    robotDo( a, b, c, abs(a*128), abs(b*128), abs(c*128));
    //delay(10);
    a+=ra;
    b+=rb;
    c+=rc;
  }
  
}

double ModeChicken::dRandom() {
  return random(-1000,1000)/1000.0;
}


void ModeChicken::robotDo(double sa, double sb, double sc, byte r, byte g, byte b) {
  servos.updateServos( getServoVal(sa), getServoVal(sb), getServoVal(sc) ); 
  brain.setLED(r,g,b);  
}

/**
 * in = 0.0 to 1.0
 out = 80-180
 */
int ModeChicken::getServoVal( double in ) {
  int val = (255*((in+1.0)/2.0));

  if ( val < 0 ) {
    val = 0;
  }
  
  if ( val > 255 ) {
    val = 255;
  }
  
  return val;
}

uint8_t ModeChicken::sin2color(double v) 
{
  return (v+1.0)*128; 
}
