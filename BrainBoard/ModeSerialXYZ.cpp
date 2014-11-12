/**
 * ModeSerialXYZ 
 *   - Mode that responds to Serial commands and programs motors using
 *     delta math.
 *
 */
#include "ModeSerialXYZ.h"
#include "BrainBoard.h"

ModeSerialXYZ::ModeSerialXYZ() {}


void ModeSerialXYZ::begin() {
  //Serial.begin(BPS);
}

void ModeSerialXYZ::serialEvent(int channel) {
 
  // Serial: We are looking only for 10 byte packets that end in '\n'
  // Serial Packet format:
  //    <'D'><'D'><CHANNEL><X><Y><Z><R><G><B><'\n'>
  //
  if ( Serial.readBytesUntil(0x0A, serialBuf, 10) > 8 ) {    
    if ( serialBuf[0]==0x44 && serialBuf[1]==0x44 && serialBuf[2] == channel ) {
      delta.compute( (byte)serialBuf[3], (byte)serialBuf[4], (byte)serialBuf[5] );
      servos.refresh();
      brain.setLED((byte)serialBuf[6],(byte)serialBuf[7],(byte)serialBuf[8]&0xff);
    }
  }
}

