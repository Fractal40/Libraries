

#ifndef ServoLib_h
#define ServoLib_h

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <avr/dtostrf.h>


class ServoLib
{
	public:
		ServoLib(int _SERVO_INDEX, int _SERVO_MIN, int _SERVO_MAX );
		void setEasingFunc(String _easingMethod);
		void setTiming(int _sweepTime, int _updateFreq);
		void setStepParabola(int _paraAmp);
		void begin(int _startingPos);
		int readServo();
		void write(int _servoTarget);
		void debugEaser();
		void arc();

	private:
		//external libraries
		Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
		//internal interrupt methods
		void interrupt_checkNewPos(int _servoTarget);
		void update();
		//easing and manipulation methods
		void easing();
		long servoEaseInOut();
		long servoEaseOut();
		long servoNoEase();
		String easingMethod; //either easeInOut, easeOut, no
		void addStepParabola();
		int paraAmp = 0;
		//timing variables
		unsigned long prevUpdate;
		int sweepTime, updateFreq;
		unsigned int determineTime;

		//Position variables
		long startPos, endPos, currPos;
		int tick, tickCount;

		//flags
		boolean arrived;
		boolean begun = false;
		boolean parabola = false;

		//servo constants
		int SERVO_INDEX;
		int SERVO_MIN;
		int SERVO_MAX;

};


#endif
