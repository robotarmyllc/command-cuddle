/* --------------------------------------------------------------------------
 * SimpleOpenNI Kinect Hand Tracking to Delta Robot over Serial
 * Tested to work on Processing v223.  Newer versions may not work.
 * --------------------------------------------------------------------------
 * For Robot Army Starter Kit robot
 * http://robot-army.com
 * --------------------------------------------------------------------------
 * Processing Wrapper for the OpenNI/Kinect 2 library
 * http://code.google.com/p/simple-openni
 * --------------------------------------------------------------------------
 * prog:  Max Rheiner / Interaction Design / Zhdk / http://iad.zhdk.ch/
 * date:  12/12/2012 (m/d/y)
 * --------------------------------------------------------------------------
 * This demo shows how to use the gesture/hand generator.
 * --------------------------------------------------------------------------
 */

import java.util.Map;
import java.util.Iterator;
import processing.serial.*;

import SimpleOpenNI.*;

SimpleOpenNI context;
float        zoomF =0.5f;
float        rotX = radians(180);  // by default rotate the hole scene 180deg around the x-axis, 
// the data from openni comes upside down
float        rotY = radians(0);
int          handVecListSize = 30;
Map<Integer, ArrayList<PVector>>  handPathList = new HashMap<Integer, ArrayList<PVector>>();
color[]       userClr = new color[] { 
  color(255, 0, 0), 
  color(0, 255, 0), 
  color(0, 0, 255), 
  color(255, 255, 0), 
  color(255, 0, 255), 
  color(0, 255, 255)
};

int pxMax =  30000;
int pxMin = -30000;
int pyMax =  30000;
int pyMin = -30000;
int pzMax =  30000;
int pzMin = -30000;

Serial myPort;  // Create object from Serial class

int nPoints = 1;
int point = 0;
PVector[] pl = new PVector[nPoints];

int xP = 0;
int yP = 0;
int zP = 0;

void setup()
{
  size(1024, 768, OPENGL);

  context = new SimpleOpenNI(this);
  if (context.isInit() == false)
  {
    println("Can't init SimpleOpenNI, maybe the camera is not connected!"); 
    exit();
    return;
  }

  // disable mirror
  context.setMirror(false);

  // enable depthMap generation 
  context.enableDepth();

  // enable hands + gesture generation
  context.enableHand();
  context.startGesture(SimpleOpenNI.GESTURE_WAVE);

  // set how smooth the hand capturing should be
  //context.setSmoothingHands(.5);

  stroke(255, 255, 255);
  smooth();

  perspective(radians(45), float(width)/float(height), 10.0f, 150000.0f);
  
  /* This section is for OSX,  needs a method for Windows.
   * Determines what FTDI serial-usb cables are present.
   * Grabs the first USB-Serial cable it finds.
   */
  // Uncomment for debug
  //println(Serial.list());
  String portName = "";
  String ports[] = Serial.list();
  for ( String p : ports) {
    if ( p.startsWith("/dev/tty.usbserial") ) {
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
  
  initPointAverages();
}

void initPointAverages() {
  for (int i=0; i<nPoints; i++) {    
    pl[i] = new PVector();
  }  
}


PVector getAveragedPoint() {
  PVector pa = new PVector();
  for (int i=0; i<nPoints; i++) {    
    pa.x += pl[i].x;
    pa.y += pl[i].y;
    pa.z += pl[i].z;
  }
  
  pa.x /= (float)nPoints;
  pa.y /= (float)nPoints;
  pa.z /= (float)nPoints;
  
  return pa;
}

PVector getCloudAvg(ArrayList<PVector> vecList) {
  PVector pa = new PVector();
  pa.x = 0;
  pa.y = 0;
  pa.z = 0;
  
  int i = 0;
  Iterator itrVec = vecList.iterator(); 
  while ( itrVec.hasNext () ) { 
    PVector p = (PVector) itrVec.next(); 
    pa.x += p.x;
    pa.y += p.y;
    pa.z += p.z;
    
    i++;
  }
  
  if ( i>0 ) {
    pa.x /= i;
    pa.y /= i;
    pa.z /= i;
  }
  
  return pa;
}

void draw()
{
  // update the cam
  context.update();

  background(0, 0, 0);

  // set the scene pos
  translate(width/2, height/2, 0);
  rotateX(rotX);
  rotateY(rotY);
  scale(zoomF);

  // draw the 3d point depth map
  int[]   depthMap = context.depthMap();
  int     steps   = 10;  // to speed up the drawing, draw every third point
  int     index;
  PVector realWorldPoint;

  translate(0, 0, -1000);  // set the rotation center of the scene 1000 infront of the camera
  //translate(0, 0, 0); 

  // draw point cloud
  stroke(200); 
  beginShape(POINTS);
  for (int y=0;y < context.depthHeight();y+=steps)
  {
    for (int x=0;x < context.depthWidth();x+=steps)
    {
      index = x + y * context.depthWidth();
      if (depthMap[index] > 0)
      { 
        // draw the projected point
        realWorldPoint = context.depthMapRealWorld()[index];
        vertex(realWorldPoint.x, realWorldPoint.y, realWorldPoint.z);
      }
    }
  } 
  endShape();

  // draw the tracked hands
  if (handPathList.size() > 0)  
  {    
    Iterator itr = handPathList.entrySet().iterator();     
    if (itr.hasNext ())
    {
      Map.Entry mapEntry = (Map.Entry)itr.next(); 
      int handId =  (Integer)mapEntry.getKey();
      ArrayList<PVector> vecList = (ArrayList<PVector>)mapEntry.getValue();
      PVector p;

      pushStyle();
      stroke(userClr[ (handId - 1) % userClr.length ]);
      noFill();           
      Iterator itrVec = vecList.iterator(); 
      beginShape();
      while ( itrVec.hasNext () ) 
      { 
        p = (PVector) itrVec.next(); 
        vertex(p.x, p.y, p.z);
      }
      endShape();   

      stroke(userClr[ (handId - 1) % userClr.length ]);
      strokeWeight(4);
      
      p = vecList.get(0);
      
      point(p.x, p.y, p.z);
      popStyle();

      PVector mp = getCloudAvg(vecList);
      
      mp.x = p.x;
      mp.y = p.y;
      mp.z = p.z;
            
      float LIMIT = 255.0;
      float DIV = 2.0;
      mp.z -= 1000;
      if ( mp.y > LIMIT )   { mp.y=LIMIT; }
      if ( mp.y < -LIMIT ) { mp.y=-LIMIT; }
      if ( mp.x > LIMIT )   { mp.x=LIMIT; }
      if ( mp.x < -LIMIT ) { mp.x=-LIMIT; }
      if ( mp.z > LIMIT )   { mp.z=LIMIT; }
      if ( mp.z < -LIMIT ) { mp.z=-LIMIT; }

      pl[point] = mp;
    
      PVector pa = getAveragedPoint();
      
      int x = (int)((-pa.x + LIMIT)/DIV);
      int y = (int)((-pa.y + LIMIT)/DIV);
      int z = (int)((pa.z + LIMIT)/DIV);
     
      // Uncomment for debug
      //println("X: " + (x&0xff) + "   Y: " + (y&0xff) + "  Z: " + (z&0xff) );
      
      if ( x < 0 ) x = 0;
      if ( x > 255 ) x = 255;
      if ( y < 45 ) y = 45;
      if ( y > 255 ) y = 255;
      if ( z < 0 ) z = 0;
      if ( z > 255 ) z = 255;
      
      byte buf[] = new byte[10];
      
      // Robot Army Delta looks for the 'DD' characters to know that
      // the following data is a real data packet and not noise.
      buf[0] = 'D';
      buf[1] = 'D';
      //buf[2] = 0x00;  // Always zero, no need to change from init-ed value.
      
      // Axis are moved around since the Kinect Z axis is toward and away from user,
      // but Robot Army bot uses  Z axis as rise and lower.  X and Y problaby flipped
      // for some other reason I forgot about.  Play with it, see what you get.
      buf[4] = (byte)(x&0xFF);   // X-axis value
      
      // Flip Kinect Z and Y Axis
      // Z becomes forward-backward
      // Y becomes up-down
      buf[3] = (byte)(z&0xFF);
      buf[5] = (byte)(y&0xFF);

      
      // LEDs
      // We sending X,Y,Z data to LEDs here for debug reasons
      // but you can send any values you want.
      buf[6] = (byte)(x&0xFF);  // Red   channel
      buf[7] = (byte)(y&0xFF);  // Blue  channel
      buf[8] = (byte)(z&0xFF);  // Green channel
      
      // End Byte
      buf[9] = 0x0A;
      
      for (int i=0; i<8; i++) {
        buf[2] = (byte)(i&0xFF);
        myPort.write(buf);
      }
      
      delay(10);
      
      // I think this was to read status back from Delta Robot.
      // Not sure if it's still used.
//      while (myPort.available() > 0) {
//        //int inByte = myPort.read();
//        print(myPort.readString());
//        //print(inByte);
//        //print(':');
//        println();
//      }
    }
  }

  // draw the kinect cam
  context.drawCamFrustum();
  
  point++;
  if (point >= nPoints) {
    point = 0;
  }
}

// These are the original methods from the Kinect Demo code that we
// started with.  You should leave these alone.
// -----------------------------------------------------------------
// hand events

void onNewHand(SimpleOpenNI curContext, int handId, PVector pos)
{
  println("onNewHand - handId: " + handId + ", pos: " + pos);

  ArrayList<PVector> vecList = new ArrayList<PVector>();
  vecList.add(pos);

  handPathList.put(handId, vecList);
}

void onTrackedHand(SimpleOpenNI curContext, int handId, PVector pos)
{
  //println("onTrackedHand - handId: " + handId + ", pos: " + pos );

  ArrayList<PVector> vecList = handPathList.get(handId);
  if (vecList != null)
  {
    vecList.add(0, pos);
    if (vecList.size() >= handVecListSize)
      // remove the last point 
      vecList.remove(vecList.size()-1);
  }
}

void onLostHand(SimpleOpenNI curContext, int handId)
{
  println("onLostHand - handId: " + handId);

  handPathList.remove(handId);
}

// -----------------------------------------------------------------
// gesture events

void onCompletedGesture(SimpleOpenNI curContext, int gestureType, PVector pos)
{
  println("onCompletedGesture - gestureType: " + gestureType + ", pos: " + pos);

  context.startTrackingHand(pos);

  int handId = context.startTrackingHand(pos);
  println("hand tracked: " + handId);
}

// -----------------------------------------------------------------
// Keyboard event
void keyPressed()
{
  switch(key)
  {
  case ' ':
    context.setMirror(!context.mirror());
    break;
  }

  switch(keyCode)
  {
  case LEFT:
    rotY += 0.1f;
    break;
  case RIGHT:
    rotY -= 0.1f;
    break;
  case UP:
    if (keyEvent.isShiftDown())
      zoomF += 0.01f;
    else
      rotX += 0.1f;
    break;
  case DOWN:
    if (keyEvent.isShiftDown())
    {
      zoomF -= 0.01f;
      if (zoomF < 0.01)
        zoomF = 0.01;
    }
    else
      rotX -= 0.1f;
    break;
  }
}

