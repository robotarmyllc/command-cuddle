/**
 *  Delta Kinetics
 *
 */
#ifndef DELTA_KINETICS_H
#define DELTA_KINETICS_H

class DeltaKinetics {
   
   public:
     DeltaKinetics();
     void begin();
     void compute(float x, float y, float z);
     float * get();
     void printData();

     float phi_prev[3];
     float phi[3];
     float diff[3];
     
   private:
     static const float a; // length of hip, flat piece
     static const float b; // length of ankle, tube
     static const float r0; // radius of base
     static const float R0; // radius of effector
    
     static const float zMax;  // Z max input (when x,y=0)
     static const float zMin;  // Z min input (when x,y=0)
    
     static const float incrementx;
     static const float incrementy;
     static const float incrementz;
    
     static const float theta[3];

     float phiA[3];
    
     float xg; //-incrementx; // position of effector
     float yg; //-incrementy;
     float zg;
    
     float r[3][3];
     float R[3][3];
    
};
 
#endif
