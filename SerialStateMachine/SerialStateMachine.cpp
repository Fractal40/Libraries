
// Library adapted by Lars Sträm from Example State
//machine reading serial input
// Author: Nick Gammon
// Date: 17 December 2011
// Modified by Lars Ström (2019-10-18) to read output
// from processing (SerialServoJoystick.pde)
// Board: Adafruit Feather M0 Proto
// Format from SerialServoJoystick = XnnYnnSnnnPnnn (n = integer)
// Start with SerialStateMachine.processIncomingByte(Serial1.read())
// get value from Serial - Array[0-3] = SerialStateMachine.getValue(0-3)

#include "Arduino.h"
#include "SerialStateMachine.h"


SerialStateMachine::SerialStateMachine()
{

}

int SerialStateMachine::getValue(int callValue)
{
  _callValue = callValue;

  return calledValue[_callValue];
}

void SerialStateMachine::processX(const unsigned int value)
{
  // do something with X
  //Serial.print ("X = ");
  //Serial.print (value);
  calledValue[0] = value; //X value
} // end of processX

void SerialStateMachine::processY(const unsigned int value)
{
  // do something with Y
  //Serial.print ("Y = ");
  //Serial.print (value);
  calledValue[1] = value; //Y value
} // end of processY

void SerialStateMachine::processS(const unsigned int value)
{
  // do something with S
  //Serial.print ("S = ");
  //Serial.print (value);
  calledValue[2] = value; //S value
} // end of processS

void SerialStateMachine::processP(const unsigned int value)
{
  // do something with P
  //Serial.print ("P = ");
  //Serial.print (value);
  calledValue[3] = value; //P value
} // end of processS


void SerialStateMachine::handlePreviousState ()
{
  switch (state)
  {
  case GOT_X:
    processX (currentValue);
    break;
  case GOT_Y:
    processY (currentValue);
    break;
  case GOT_S:
    processS (currentValue);
    break;
  case GOT_P:
    processP (currentValue);
    break;
  }  // end of switch

  currentValue = 0;
}  // end of handlePreviousState

void SerialStateMachine::processIncomingByte (const byte c)
{
  if (isdigit (c))
  {
    currentValue *= 10;
    currentValue += c - '0';
  }  // end of digit
  else
  {

    // The end of the number signals a state change
    handlePreviousState ();

    // set the new state, if we recognize it
    switch (c)
    {
    case 'X':
      state = GOT_X;
      break;
    case 'Y':
      state = GOT_Y;
      break;
    case 'S':
      state = GOT_S;
      break;
    case 'P':
      state = GOT_P;
      break;
    default:
      state = NONE;
      break;
    }  // end of switch on incoming byte
  } // end of not digit

} // end of processIncomingByte

void SerialStateMachine::debug()
{

  for (int i=0;i<4;i++) {
    Serial.print(calledValue[i]);
    Serial.print(" : ");
  }
  Serial.println();

}
