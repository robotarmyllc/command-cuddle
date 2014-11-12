/**
 *  Mode Library -- Common routines for animating the robot.
 *
 */
#ifndef MODE_LIB_H
#define MODE_LIB_H

#include "Arduino.h"
#include "DeltaServos.h"

class ModeLibrary {
  public:
    ModeLibrary();
    void begin();

    double servoA, servoB, servoC;  // Current position  -1.0 to 1.0 of the servos.
    int ledR, ledG, ledB;  // Current intesity of the RGB LED colors.  (0-255)
    int ledRDir, ledGDir, ledBDir;  // Current amount to add or subtract from LED intensity
    int maxR, maxG, maxB;  // Max intensity of the LED
    int minR, minG, minB;  // Min intensity of the LED
    int incR, incG, incB;  // Add or Subtract this amount from the current value each cycle.

    void configLED( int inR, int inG, int inB );  //  Usually 1 or 2
    void configLED( 
          int mnR, int mnG, int mnB,  // Usually zero
          int mxR, int mxG, int mxB,  // No more than 255
          int inR, int inG, int inB   //  Usually 1 or 2
       );
    void fadeLED();
    void pause(int mn,int ms);
    void twitch();
    void bob(int nBobs, int pmn1, int pmx1, int pmn2, int pmx2);
    //void bob2();
    void jab();
    void mope();
    void perk();
    void swirl( int rate );

    int  getServoVal( double in );
    void robotRandom(int steps );
    void robotRamp( int steps, double sa, double sb, double sc );
     int coinFlip(); // Return a random number between 0 and 1.
    void rangeCheckServos(void);
    void updateServos(void);

  private:
    double dRandom();
    void robotDo(double sa, double sb, double sc, byte r, byte g, byte b) ;
};

#endif

