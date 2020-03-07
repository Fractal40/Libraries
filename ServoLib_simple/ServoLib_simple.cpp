
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoLib_simple.h"
#include <avr/dtostrf.h>

ServoLib_simple::ServoLib_simple(int _SERVO_INDEX, int _SERVO_MIN, int _SERVO_MAX )
{
	SERVO_INDEX = _SERVO_INDEX;
	SERVO_MIN = _SERVO_MIN;
	SERVO_MAX = _SERVO_MAX;
}

void ServoLib_simple::begin(int _startingPos, int _PWMFreq)
{

	if (_startingPos > SERVO_MIN && _startingPos < SERVO_MAX) {
		begun = true;
	} else {
		begun = false;
		while (begun = false)
		{
			Serial.println("No valid starting value");
		}
	}

	//Adafruit_PWMServoDriver library begin()
	pwm.begin();
	delay(1000);
	pwm.setPWMFreq(_PWMFreq);

	//default values
	sweepTime = 0;
	updateFreq = 0;
	tickCount = sweepTime/updateFreq;
	startPos = _startingPos;
	endPos = _startingPos;
	currPos = _startingPos;
	pwm.setPWM(SERVO_INDEX, 0, endPos); //initial position of servo (pulse = 350 --> 90; 90; 90 deg)

}

void ServoLib_simple::write(int _servoTarget)
{
	_servoTarget = err(_servoTarget);
	interrupt_checkNewPos(_servoTarget);
	update_simple();
}


void ServoLib_simple::write(int _servoTarget, int _sweepTime, int _updateFreq)
{

	sweepTime = _sweepTime;
	updateFreq = _updateFreq;
	tickCount = _sweepTime/_updateFreq;
	_servoTarget = err(_servoTarget);
	interrupt_checkNewPos(_servoTarget);
	update_sweep();
}

void ServoLib_simple::update_simple()
{
	if (endPos != startPos) {
      arrived = false;
  } else {
      arrived = true;
  }

  if( arrived == false) {

		//if (tick == 1) determineTime = millis();
		currPos = endPos;
		pwm.setPWM(SERVO_INDEX, 0, currPos);
    //debugEaser();
		startPos = endPos;

  }

    //debugEaser();

}

void ServoLib_simple::update_sweep()
{
  if (millis() - prevUpdate < updateFreq) {

    return;
  }
  prevUpdate = millis();

    if (endPos != startPos) {
      arrived = false;
    } else {
      arrived = true;
    }

    if( arrived == false) {
      tick++;

	    //if (tick == 1) determineTime = millis();
			currPos = getNewpos();
			pwm.setPWM(SERVO_INDEX, 0, currPos);
      //debugEaser();

    }

    //debugEaser();

    if( tick == tickCount ) {
      arrived = true;
      tick = 0;
      //Serial.print(millis()-determineTime+20); Serial.println(" ms");
    }

    if (arrived == true) {
       startPos = endPos;
    }


}

int ServoLib_simple::getNewpos()
{

  float c = (endPos - startPos);
  float t = (updateFreq/(sweepTime*1.0))*(tick)*sweepTime;
  t /= sweepTime/1.0;

  return c*t + startPos;
}

void ServoLib_simple::interrupt_checkNewPos(int _servoTarget)
{
  if (endPos != _servoTarget) {
    tick = 0;
    arrived = true;
    startPos = currPos;
    endPos = _servoTarget;
  }
}

void ServoLib_simple::debugEaser()
{
  //static char disp1[20]; dtostrf(c, 5,0,disp1);
  //static char disp2[20]; dtostrf(t, 5,3,disp2);

  Serial.print(tick); Serial.print(" : ");
  Serial.print(tickCount); Serial.print(" : ");
  Serial.print(int(arrived)); Serial.print(" : ");
  Serial.print(startPos); Serial.print(" : ");
  Serial.print(currPos); Serial.print(" : ");
  Serial.print(endPos); Serial.print(" : ");
  Serial.println();

}

int ServoLib_simple::isRunning()
{
	return 1-int(arrived);
}

int ServoLib_simple::read()
{
	return currPos/1000;
}

int ServoLib_simple::err(int errVal)
{
	if (errVal < SERVO_MIN || errVal > SERVO_MAX) {
		Serial.print(errVal); Serial.print(": ");
		Serial.println("target outside accepted interval");
	}
		return constrain(errVal, SERVO_MIN, SERVO_MAX); //checks and constrains value to an acceptable one

}
