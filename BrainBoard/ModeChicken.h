/**
 *  Mode -- Chicken
 *
 */
#ifndef MODE_CHICKEN_H
#define MODE_CHICKEN_H

#include <math.h>
#include "Arduino.h"
#include "DeltaServos.h"

class ModeChicken {
  public:
    ModeChicken();
    void begin();
    void update();

  private:
    void robotSwirl();
    void robotChicken();
    void robotBob();
    void robotRandom();
    void robotRamp( int steps, double a1, double b1, double c1, double a2, double b2, double c2 );
    double dRandom();
    void robotDo(double sa, double sb, double sc, byte r, uint8_t g, uint8_t b) ;
    int getServoVal( double in );
    uint8_t sin2color(double v);

    int frame;
    const static int maxframes = (int)(100*2*M_PI);
    //const static int servoRange = DeltaServos::MAX_SERVO-DeltaServos::MIN_SERVO;
    //const static int servoMid = DeltaServos::MIN_SERVO + (servoRange/2);

};

#endif
                
