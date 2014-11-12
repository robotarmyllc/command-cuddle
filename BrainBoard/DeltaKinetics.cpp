/**
 * Delta Kinectics Module
 *
 */

#include "DeltaKinetics.h" 
#include "DeltaServos.h"
#include "Arduino.h"

DeltaKinetics::DeltaKinetics() {}

const float DeltaKinetics::theta[3] = { 0, 120*M_PI/180, -120*M_PI/180 };

void DeltaKinetics::begin() {
  xg = 0; // position of effector
  yg = 0; 
  zg = 0;
  
//  theta[0] =  0;
//  theta[1] =  120*M_PI/180;
//  theta[2] = -120*M_PI/180;
    
  phi_prev[0] = 127;
  phi_prev[1] = 127;
  phi_prev[2] = 127;
    
  phi[0] = 127;
  phi[1] = 127;
  phi[2] = 127;
}

void DeltaKinetics::compute( float x, float y, float z ){
  xg = map(x, 0, 255, -2000, 2000)/1000.0;
  yg = map(y, 0, 255, -2000, 2000)/1000.0;
  zg = map(z, 0, 255, zMin*1000, zMax*1000)/1000.0;
  
  for (int i=0 ; i<3; i++){
    r[i][0] = 0;
    r[i][1] = r0*cos(theta[i]);
    r[i][2] = r0*sin(theta[i]);
    R[i][0] = zg + 0; 
    R[i][1] = xg + R0*cos(theta[i]);
    R[i][2] = yg + R0*sin(theta[i]);
  }
  for (int i=0; i<3; i++){
    float Cr =  (R[i][1]-r[i][1])*cos(theta[i]) + (R[i][2]-r[i][2])*sin(theta[i]);
    float Ct = -(R[i][1]-r[i][1])*sin(theta[i]) + (R[i][2]-r[i][2])*cos(theta[i]);
    float Cz = zg;
    float A = a;
    float B = sqrt(b*b - Ct*Ct);
    float C = sqrt(Cr*Cr + Cz*Cz);
    float alpha = atan2(Cz,Cr)*180/M_PI;
    if (abs((A*A+C*C-B*B)/(2*A*C))<=1){   
      float beta = acos((A*A+C*C-B*B)/(2*A*C))*180/M_PI;
      phiA[i] = -(alpha-beta);
      //phiA[i] = constrain(phiA[i],-80.0,80.0);
      phiA[i] = constrain(phiA[i],-40,40);
      phi[i]=map(phiA[i],-40.0,40.0,DeltaServos::MIN_SERVO,DeltaServos::MAX_SERVO); // Servo range
      //phi[i]=map(phiA[i],-70,70,0,255); // Servo range
    } else {
      phi[i] = phi_prev[i];
      //Serial.println("Warning: Position Out of Range");
    }
    phi_prev[i] = phi[i];
  }
  //printData();
  //delay(500);
}

float * DeltaKinetics::get() {
  return phi;
}

void DeltaKinetics::printData() {
  for (int i=0; i<3; i++){
    Serial.print("anglecurrent:  ");
    Serial.print(phiA[i]);
    Serial.print("\t anglemapped: "); 
    Serial.println(phi[i]);
  }
}




