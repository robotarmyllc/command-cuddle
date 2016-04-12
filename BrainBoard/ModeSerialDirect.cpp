/**
 * ModeSerialDirect 
 *   - Mode that responds to Serial commands and programs motors directly
 *
 */
#include "ModeSerialDirect.h"
#include "BrainBoard.h"
//#include "Serial.h"   // Not needed in Arduino 1.6 and above

ModeSerialDirect::ModeSerialDirect() {}


void ModeSerialDirect::begin() {
  //Serial.begin(BPS);
}

void ModeSerialDirect::serialEvent() {
 
  // Serial: We are looking only for 10 byte packets that end in '\n'
  // Serial Packet format:
  //    <'D'><'D'><CHANNEL><X><Y><Z><R><G><B><'\n'>
  //
  if ( Serial.readBytesUntil(0x0A, serialBuf, 10) > 8 ) {  // Nine bytes ending with 0x0A
    if ( serialBuf[0]==0x44 && serialBuf[1]==0x44 ) {  // And only if it has our header of 'DD'
      servos.updateServos( (byte)serialBuf[3], (byte)serialBuf[4], (byte)serialBuf[5] );  // Three servo position bytes
      brain.setLED((byte)(serialBuf[6]&0xff),(byte)(serialBuf[7]&0xff),(byte)(serialBuf[8]&0xff)); // and RGB bytes
    }
  }
}

