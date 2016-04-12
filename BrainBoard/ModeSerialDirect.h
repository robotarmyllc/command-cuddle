/**
 *  Serial Direct Mode
 *  
 *  serialEvent() programs direct motor positions.
 *  No delta computation.
 */

#ifndef MODE_SERIAL_DIRECT_H
#define MODE_SERIAL_DIRECT_H

//#include <Serial.h>

class ModeSerialDirect {
  public:
    ModeSerialDirect();
    void begin();
    void serialEvent();

   private:
     const static int BPS = 57600;
     char serialBuf[10];
};

#endif
                

