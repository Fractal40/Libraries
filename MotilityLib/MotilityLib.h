#ifndef MotilityLib_h
#define MotilityLib_h

#include "Arduino.h"
#include <avr/dtostrf.h>


class MotilityLib
{
    public:
      MotilityLib(int _startingPos);
      void init();
      void setTiming(int _sweepTime, int _updateFreq);
      void setEasingFunc(String _easingMethod);
      void easing();
      void send(int _newPos);
      void update();
      int getPos();
      void debugEaser();


    private:
      long easeInOut();
      long easeOut();
      long noEase();
      String easingMethod;

      long startingPos, startPos, currPos, endPos;

      //timing variables
      unsigned long prevUpdate;
      int sweepTime, updateFreq, tickCount, tick;
      unsigned int determineTime;

      //flags
      boolean arrived;

};

#endif
