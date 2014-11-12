#ifndef CC_SDK_H
#define CC_SDK_H
#ifdef MAINFILE
    #define EXTERN
#else
    #define EXTERN extern
#endif

//#define REV_D

#include "DeltaBrain.h"
#include "DeltaServos.h"
#include "DeltaKinetics.h"
#include "Modes.h"
#include "ModeChicken.h"
//#include "ModeAngry.h"
//#include "ModeHappy.h"
//#include "ModeCaffeinated.h"
//#include "ModeSad.h"
#include "ModeSerialDirect.h"
#include "ModeSerialXYZ.h"

EXTERN  DeltaBrain    brain;
EXTERN  DeltaKinetics delta;
EXTERN  DeltaServos   servos;

EXTERN  Modes         mode;

EXTERN  ModeChicken         modeChicken;
//EXTERN  ModeAngry           modeAngry;
//EXTERN  ModeHappy           modeHappy;
//EXTERN  ModeCaffeinated     modeCaffeinated;
//EXTERN  ModeSad             modeSad;
EXTERN  ModeSerialDirect    modeSerialDirect;
EXTERN  ModeSerialXYZ       modeSerialXYZ;

#endif

