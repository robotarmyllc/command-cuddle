/**
 * Modes :: A collection of emotive modes for the robot to perform.
 *
 */
#include "Modes.h"
#include "BrainBoard.h"

Modes::Modes() {
  lib.begin();
}


/******************************************************************
 * Angry Mode
 */
void Modes::angryInit() {
  lib.configLED( 16,0,0 ); // Make only the Red LED fade in and out.
}

void Modes::angryUpdate(){
  for ( int i=0; i<(rand()%3)+1; i++) {
   lib.twitch();
  }
  if ( lib.coinFlip() ) {
    lib.jab();
  }
  lib.pause(30, 500);

  lib.robotRandom((rand()%20) + 5);
  if ( lib.coinFlip() ) {
    lib.jab();
  }
  lib.pause(30, 1000);
  if ( lib.coinFlip() ) {
    lib.bob(2, 1, 300, 1, 100 );    
  }
  
  if ( lib.coinFlip() ) {
    lib.jab();
    lib.pause(100,100);
  }
}


/******************************************************************
 * Happy Mode
 */
void Modes::happyInit() {
  lib.configLED( 2,2,0 ); // Make only the Red and Green LEDs (Red+Green = Yellow) fade in and out.
}

void Modes::happyUpdate() {
  int t = (rand()%8)+2;
  for ( int i=0; i<t; i++) {
    lib.robotRandom((rand()%70)+20);
    lib.pause(30, 100);
  }
  
  if ( lib.coinFlip() ) {
    // Perk up
    lib.robotRamp(40, 0.6, 0.8, 0.8 );
    lib.swirl( 7 );
  }
   
  // Move closer to center and higher when we bob.
  lib.robotRamp(40, 1.0, 1.0, 1.0 );
  // Bob one to four times.
  t = (rand()%4);
  for( int i=0; i<t; i++) {
    lib.bob(10, 1, 150, 1, 100 );
  }
}


/******************************************************************
 * Caffeinated Mode
 */
void Modes::caffeinatedInit() {
  lib.configLED( lib.coinFlip(), lib.coinFlip(), lib.coinFlip() );
}

void Modes::caffeinatedUpdate(){
  if ( lib.coinFlip() ) {
    lib.twitch();
  }
  lib.configLED( lib.coinFlip(), lib.coinFlip(), lib.coinFlip() );
  lib.robotRandom((rand()%40) + 5);
  lib.configLED( lib.coinFlip(), lib.coinFlip(), lib.coinFlip() );
  if ( lib.coinFlip() ) {
    lib.pause(30,300);
    lib.robotRandom((rand()%40) + 5);
  }
  lib.pause(5,200);
  if (rand()%6 == 5) {
    int t = (rand()%5) + 1;
    for ( int i=0; i<t; i++ ) {
      lib.configLED( lib.coinFlip(), lib.coinFlip(), lib.coinFlip() );
      lib.twitch();
      lib.pause(5, 100);
    }
    lib.pause(100,400);

  }
}


/******************************************************************
 * Sad Mode
 */
void Modes::sadInit() {
  lib.configLED( 0,0,1 ); // Make only the Blue LED fade in and out.
}

void Modes::sadUpdate() {  
  lib.perk();
  lib.pause(400,800);
  
  for(int i=0; i<rand()%5; i++ ) {
    lib.pause(300,1000);
    lib.robotRandom(random(60,150));
  }
  delay(rand()%1600+400);
  lib.pause(100,500);
  lib.mope();
  lib.pause(5000,8000);
}

