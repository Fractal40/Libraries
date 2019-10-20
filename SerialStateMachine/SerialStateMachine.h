// Library adapted by Lars Sträm from Example State
//machine reading serial input
// Author: Nick Gammon
// Date: 17 December 2011
// Modified by Lars Ström (2019-10-18) to read output
// from processing (SerialServoJoystick.pde)
// Board: Adafruit Feather M0 Proto


#ifndef SerialStateMachine_h
#define SerialStateMachine_h

#include "Arduino.h"



class SerialStateMachine
{
  public:
    SerialStateMachine();
    void processIncomingByte(const byte c);
    int getValue(int callValue);
    void debug();


  private:

    void processX(const unsigned int value);
    void processY(const unsigned int value);
    void processS(const unsigned int value);
    void processP(const unsigned int value);
    void handlePreviousState ();

    typedef enum {  NONE, GOT_X, GOT_Y, GOT_S, GOT_P } states;
    states state = NONE;
    int _callValue;
    int calledValue[4];
    unsigned int currentValue;

};

#endif
