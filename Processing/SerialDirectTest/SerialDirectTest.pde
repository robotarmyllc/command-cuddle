/* 
 * Robot Army Start Kit -- http://robot-army.com
 * Serial Direct Test - 
 *     Direct positioning of the motors.
 *     No Delta math is performed.
 *
 *  NOTICE: This code is known to work on Processing 2.1
 *  It has not been tested on other versions.
 *
 *
 *  This is some simple code to test the serial connection between
 *  the delta robot and your computer.  You can use this code as a 
 *  starting point for your own code.
 *
 *  Set your delta switches to UP/DN/DN/DN to use this mode.
 *  WARNING: Don't use this test for other delta modes.
 *
 *  NOTE: It may help to remove the 'SER_RST' jumper on the brain board.
 *  The FTDI cable sends a reset signal when this program starts or stops.
 *  Removing the SER_RST jumper will prevent resets from occuring.
 *  Just remember to put the jumper back when you do any Brain/Arduino programming.

=======================================================================================
Copyright (c) 2014, Robot Army LLC     All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS  SOFTWARE IS PROVIDED  BY THE  COPYRIGHT  HOLDERS  AND CONTRIBUTORS  "AS IS"  AND
ANY  EXPRESS  OR  IMPLIED WARRANTIES,  INCLUDING,  BUT  NOT  LIMITED  TO,  THE IMPLIED 
WARRANTIES  OF MERCHANTABILITY  AND FITNESS FOR A  PARTICULAR PURPOSE  ARE DISCLAIMED. 
IN  NO  EVENT SHALL THE  COPYRIGHT HOLDER OR  CONTRIBUTORS  BE LIABLE  FOR ANY DIRECT, 
INDIRECT, INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL DAMAGES  (INCLUDING, BUT
NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES;  LOSS OF USE,  DATA, OR
PROFITS;  OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON  ANY THEORY OF  LIABILITY, 
WHETHER IN CONTRACT,  STRICT LIABILITY,  OR  TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. 
=======================================================================================
*/

import processing.serial.*;

Serial myPort;  // Create object from Serial class

int pos = 0;

void setup() {  
  //println(Serial.list());  // Uncomment this to list out your serial ports.
  String portName = "";
  String ports[] = Serial.list();
  for ( String p : ports) {
    // This code is for Mac.  It will find a FTDI serial adapter if you have
    // one plugged in.
    if ( p.startsWith("/dev/tty.usbserial") ) {
      portName = p;
      println("Serial:  " + p);
    }
        
    // Windows.  hardcoded to COM4 change it to whatever yours comes up as.
    // Uncomment the println() at the top of this method to list
    // out your ports if it is not COM4.
    if ( p.equals("COM4") ) {
      portName = p;
      println("Serial:  " + p);
    }
  }

  if ( portName != "" ) {
    myPort = new Serial(this, portName, 57600);
  } else {
    println("No serial port found!!");
    exit();
  }
}

void draw() {
    byte buf[] = new byte[10];
    
    // This ten byte packet is the magic sauce for talking to
    // the delta over serial.
    
    // Our packet header is two D's in a row for Delta-Delta
    buf[0] = 'D';
    buf[1] = 'D';
      
    // Channel number.   
    //     Ignored by robot for Direct Motor mode.
    buf[2] = (byte)(0&0xff);
      
    // position for the three motors
    buf[3] = (byte)(pos&0xFF);  // Motor 1 position  (0-255)
    buf[4] = (byte)(pos&0xFF);  // Motor 2 position  (0-255)
    buf[5] = (byte)(pos&0xFF);  // Motor 3 position  (0-255)
      
    buf[6] = (byte)(pos&0xFF);  //   Red LED intensity  (0-255)
    buf[7] = (byte)(pos&0xFF);  // Green LED intensity  (0-255)
    buf[8] = (byte)(pos&0xFF);  //  Blue LED intensity  (0-255)
      
    // Terminate character
    buf[9] = 0x0A;
      
    // Write the buffer over serial.
    myPort.write(buf);
    
    // Wait when we are at position 0.
    if ( pos == 0 ) delay(700);
    
    // Delta should rise slowly then lower back down.
    pos++;
     if ( pos > 255 ) pos = 0;
}

