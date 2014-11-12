/**
 * DeltaServos - Class to handle the three Delta Servos
 *
 */
#include "BrainBoard.h"
#include "DeltaServos.h"
#include "Arduino.h"

DeltaServos::DeltaServos() {
  led_r_val = 0;
  led_g_val = 0;
  led_b_val = 0;
  
  int servo_dir = -1;
  int servo_delay = 15;
}

DeltaServos::~DeltaServos() { }

Servo DeltaServos::get( int servo ) {
  switch ( servo ) {
    case 1: return servoA;
    case 2: return servoB;
    case 3: return servoC;
    default: return servoA;
  }
}

void DeltaServos::begin() { // TODO: Pass the three pins here?
  servoA.attach(9); // attaches servo A to pin 9  
  servoB.attach(10); // attaches servo B to pin 10 
  servoC.attach(11); // attaches servo C to pin 11
}

void DeltaServos::updateServos( float sa, float sb, float sc ) {
  updateServos( (int)sa, (int)sb, (int)sc );
}

void DeltaServos::updateServos( float * abc ) {
  updateServos(abc[0], abc[1], abc[2]);
}

void DeltaServos::updateServos( int sa, int sb, int sc ) {
  delta.phi[0] = map(sa,0,255, MIN_SERVO, MAX_SERVO);
  delta.phi[1] = map(sb,0,255, MIN_SERVO, MAX_SERVO);
  delta.phi[2] = map(sc,0,255, MIN_SERVO, MAX_SERVO);
  
  refresh();
}

void DeltaServos::refresh() {
  servoA.write(delta.phi[0]);   // tell servo to go to positions in variable 'phi' 
  servoB.write(delta.phi[1]);
  servoC.write(delta.phi[2]);
  delay(servoDelay); 
}

//void DeltaServos::hold() {  
//  /* Holds the servo at 90 degrees to attach delta robot
//   Attach horn parallel to servo */
//  for (int i=0; i<3; i++){
//    delta.phi[i]=90;
//  }
//  refresh();
//}

//void DeltaServos::printData() {
//  Serial.print("angle1current:  ");
//  Serial.print(delta.phi[0]);
//  Serial.print("  angle1prev:   "); 
//  Serial.print(delta.phi_prev[0]);
//  for (int i = 0; i<3; i++){
//    delta.diff[i]=delta.phi[i] - delta.phi_prev[i];
//    delta.phi_prev[i]=delta.phi[i];
//  }
//  Serial.print("  diff1:  ");
//  Serial.println(delta.diff[0]);
//}

//void DeltaServos::initArms() {
//  int pos = MIN_SERVO;
//  updateServos(pos,pos,pos);
//  delay(800);  // Wait to get all the way there.
//  
//  while(pos <= 255 ) {
//    pos += servo_dir;
//    led_r_val += led_r_dir;
//    led_g_val += led_g_dir;
//    led_b_val += led_b_dir;
//    
//    int r = map(led_r_val, MIN_SERVO, MAX_SERVO, 0, 179);
//    int g = map(led_g_val, MIN_SERVO, MAX_SERVO, 0, 179);
//    int b = map(led_b_val, MIN_SERVO, MAX_SERVO, 0, 179);
//
//    updateServos(pos,pos,pos);
//    brain.setLED(r, g, b);
//  }
//  delay(1000);
//  while(pos >= 0) {
//    pos -= servo_dir;
//    led_r_val -= led_r_dir;
//    led_g_val -= led_g_dir;
//    led_b_val -= led_b_dir;
//  
//    int r = map(led_r_val, MIN_SERVO, MAX_SERVO, 0, 179);
//    int g = map(led_g_val, MIN_SERVO, MAX_SERVO, 0, 179);
//    int b = map(led_b_val, MIN_SERVO, MAX_SERVO, 0, 179);
//
//    updateServos(pos,pos,pos);
//    brain.setLED(r, g, b);
//  } 
//
//  delay(100);
//  brain.setLED(0, 0, 250);
//  delay(100);
//  brain.setLED(0, 0, 0);
//  delay(900);
//  brain.setLED(0, 0, 250);
//  delay(100);
//  brain.setLED(0, 0, 0);
//  delay(900);
//  brain.setLED(0, 0, 250);
//  delay(100);
//  brain.setLED(0, 0, 0);
//  delay(900);
//  brain.setLED(0, 0, 250);
//  delay(100);
//  brain.setLED(0, 0, 0);
//  delay(900);
//  
//  brain.setLED(250,250,250);
//}

void DeltaServos::upDown( int cycleTime ) {
  //int STEPS = 32;
  int STEPS = 255;
  int INCR = 255/STEPS;
  int dly = (cycleTime/STEPS/2);
  if (dly < servoDelay) dly = servoDelay;
  
  int i = 0;
  
  while ( i < 255 ) {
    brain.setLED( i,i,i );
    updateServos( i,i,i );
    delay(dly-servoDelay);
    i+= INCR;
  }
  delay(500);
  i = 255;
  while ( i > 0 ) {
    brain.setLED( i,i,i );
    updateServos( i,i,i );
    delay(dly-servoDelay);
    i-= INCR;
  }
  delay(500);
  
}
