
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoLib.h"
#include <avr/dtostrf.h>

ServoLib::ServoLib(int _SERVO_INDEX, int _SERVO_MIN, int _SERVO_MAX )
{
	SERVO_INDEX = _SERVO_INDEX;
	SERVO_MIN = _SERVO_MIN*1000;
	SERVO_MAX = _SERVO_MAX*1000;
}

void ServoLib::begin(int _startingPos)
{
	if (_startingPos*1000 > SERVO_MIN && _startingPos*1000 < SERVO_MAX) {
		begun = true;
	} else {
		begun = false;
		while (begun = false)
		{
			Serial.println("No valid starting value");
		}
	}
	//default values
	sweepTime = 1000;
	updateFreq = 20;
	tickCount = sweepTime/updateFreq;
	startPos = _startingPos*1000;
	endPos = _startingPos*1000;
	currPos = _startingPos*1000;
	pwm.setPWM(SERVO_INDEX, 0, endPos/1000); //initial position of servo (usually pulse = 350)

}

void ServoLib::setEasingFunc(String _easingMethod)
{
	easingMethod = _easingMethod;
}

void ServoLib::easing()
{
	if (easingMethod == "easeInOut") {
	currPos = servoEaseInOut();
} else if (easingMethod == "easeOut") {
	currPos = servoEaseOut();
} else if (easingMethod == "noEasing") {
	currPos = servoNoEase();
} else {
	currPos = servoNoEase();
} //end else if

}

void ServoLib::setTiming(int _sweepTime, int _updateFreq)
{
	sweepTime = _sweepTime;
	updateFreq = _updateFreq;
	tickCount = sweepTime/updateFreq;
}

void ServoLib::addArc(int _arcAmp)
{
	arcAmp = _arcAmp;
	if (arcAmp > 0) {
		arc = true;
	} else {
		arc = false;
	}
}

int ServoLib::read()
{
	return currPos/1000;
}

void ServoLib::write(int _servoTarget)
{
	err(_servoTarget);
	arc = false;
	interrupt_checkNewPos(_servoTarget);
	update();
}

void ServoLib::write(int _servoTarget, int _sweepTime)
{
	err(_servoTarget);
	sweepTime = _sweepTime;
	arc = false;
	interrupt_checkNewPos(_servoTarget);
	update();
}

void ServoLib::write(int _servoTarget, int _sweepTime, int _arcAmp)
{
	err(_servoTarget);
	sweepTime = _sweepTime;
	arcAmp = _arcAmp;
	if (arcAmp > 0) {
		arc = true;
	} else {
		arc = false;
	}
	interrupt_checkNewPos(_servoTarget);
	update();
}

void ServoLib::update()
{
  if (millis() - prevUpdate < updateFreq) {

    return;
  }
  prevUpdate = millis();

    if (endPos != startPos || arc == true) {  //remove arc condition  || arc == true
      arrived = false;
    } else {
      arrived = true;
			arc = false;
    }

		easing();

    if( arrived == false) {
      tick++;
      //if (tick == 1) determineTime = millis();
			arcEq();
			currPos = (currPos/1000)*1000; //constrain(currPos/1000, SERVO_MIN, SERVO_MAX);
			pwm.setPWM(SERVO_INDEX, 0, int(currPos/1000));
      //debugEaser();
    }

    //debugEaser();

    if( tick == tickCount ) {
      arrived = true;
      tick = 0;
			arc = false;
      //Serial.print(millis()-determineTime+20); Serial.println(" ms");
    } else {
      //arrived = false;

    }

    if (arrived == true) {
       startPos = endPos;
    }


}

long ServoLib::servoEaseInOut()
{

  float c = (endPos - startPos);
  float t = (updateFreq/(sweepTime*1.0))*(1.0+tick)*sweepTime;
  t /= sweepTime/2.0;
  if (t < 1) return c/2*t*t*t + startPos;
  t -= 2.0;
  return c/2*(t*t*t + 2) + startPos;

}

long ServoLib::servoEaseOut()
{

  float c = (endPos - startPos);
  float t = (updateFreq/(sweepTime*1.0))*(1.0+tick)*sweepTime;
  t /= sweepTime/1.0;

  return -c*t*(t-2) + startPos;

}

long ServoLib::servoNoEase()
{

  float c = (endPos - startPos);
  float t = (updateFreq/(sweepTime*1.0))*(1.0+tick)*sweepTime;
  t /= sweepTime/1.0;

  return c*t + startPos;
}

void ServoLib::arcEq()
{
	currPos += -1*arcAmp*pow(tick,2)+arcAmp*tickCount*(tick);
}

void ServoLib::interrupt_checkNewPos(int _servoTarget)
{
  if (endPos != _servoTarget*1000) {
    tick = 0;
    arrived = true;
    startPos = currPos;
    endPos = _servoTarget*1000;
  }
}

void ServoLib::debugEaser()
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

int ServoLib::isRunning()
{
	return 1-int(arrived);
}

int ServoLib::err(int errVal)
{
	if (errVal*1000 < SERVO_MIN || errVal*1000 > SERVO_MAX) {
		Serial.print(errVal); Serial.print(": ");
		Serial.println("target outside accepted interval");
	}
		return constrain(errVal, SERVO_MIN, SERVO_MAX); //checks and constrains value to an acceptable one

}
