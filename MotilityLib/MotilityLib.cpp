

#include "Arduino.h"
#include "MotilityLib.h"
#include <avr/dtostrf.h>


MotilityLib::MotilityLib(int _startingPos)
{
	startingPos = _startingPos*1000;
}

void MotilityLib::init()
{

	//default values
	sweepTime = 2000;
	updateFreq = 20;
	tickCount = sweepTime/updateFreq;
	startPos = startingPos;
	endPos = startingPos;
	currPos = startingPos;
	//pwm.setPWM(SERVO_INDEX, 0, endPos/1000); //initial position of servo (pulse = 350 --> 90; 90; 90 deg)

}

void MotilityLib::setTiming(int _sweepTime, int _updateFreq)
{
	sweepTime = _sweepTime;
	updateFreq = _updateFreq;
	tickCount = sweepTime/updateFreq;
}

int MotilityLib::getPos()
{
	return currPos/1000;
}

void MotilityLib::send(int _newPos)
{
	endPos = _newPos*1000;
	update();
}

void MotilityLib::update()
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

  //arcEq();
  //noEase();

  if( arrived == false) {
    tick++;
    //if (tick == 1) determineTime = millis();

    easing();
    //pwm.setPWM(SERVO_INDEX, 0, int(currPos/1000));
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

void MotilityLib::setEasingFunc(String _easingMethod)
{
	easingMethod = _easingMethod;
}

void MotilityLib::easing()
{
	if (easingMethod == "easeInOut") {
	currPos = easeInOut();
	} else if (easingMethod == "easeOut") {
	currPos = easeOut();
	} else if (easingMethod == "noEasing") {
	currPos = noEase();
	} else {
	currPos = noEase();
	} //end else if

}

long MotilityLib::easeInOut()
{

  float c = (endPos - startPos);
  float t = (updateFreq/(sweepTime*1.0))*(1.0+tick)*sweepTime;
  t /= sweepTime/2.0;
  if (t < 1) return c/2*t*t*t + startPos;
  t -= 2.0;
  return c/2*(t*t*t + 2) + startPos;

}

long MotilityLib::easeOut()
{

  float c = (endPos - startPos);
  float t = (updateFreq/(sweepTime*1.0))*(1.0+tick)*sweepTime;
  t /= sweepTime/1.0;

  return -c*t*(t-2) + startPos;

}

long MotilityLib::noEase()
{

  float c = (endPos - startPos);
  float t = (updateFreq/(sweepTime*1.0))*(1.0+tick)*sweepTime;
  t /= sweepTime/1.0;

  return c*t + startPos;
}

void MotilityLib::debugEaser()
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
