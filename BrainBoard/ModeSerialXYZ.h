/**
 *  Serial XYZ Mode
 *  
 *  serialEvent() programs delta computed motor positions.
 */

#ifndef MODE_SERIAL_XYZ_H
#define MODE_SERIAL_XYZ_H

// Not needed in Arduino 1.6 and above
//#include <Serial.h>

class ModeSerialXYZ {
  public:
    ModeSerialXYZ();
    void begin();
    void serialEvent(int channel);

   private:
     const static int BPS = 57600;
     char serialBuf[10];
};

#endif
                

