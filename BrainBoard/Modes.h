/**
 *  Modes -- Emotive modes for the robot.
 *
 */
#ifndef MODES_H
#define MODES_H

#include "ModeLibrary.h"

class Modes {
  public:
    Modes();
    void angryInit();
    void angryUpdate();
    void happyInit();
    void happyUpdate();
    void caffeinatedInit();
    void caffeinatedUpdate();
    void sadInit();
    void sadUpdate();

  private:
    ModeLibrary lib;
};

#endif

