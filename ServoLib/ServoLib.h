

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
		void addArc(int _arcAmp);
		void stop();
		void begin(int _startingPos);
		int read();
		void write(int _servoTarget);
		void write(int _servoTarget, int _sweepTime);
		void write(int _servoTarget, int _sweepTime, int _arcAmp);
		void debugEaser();

		int err(int errVal);
		int isRunning();

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

		String easingMethod; //either easeInOut, easeOut, noEasing
		long arcEq();
		long arcSine();
		int arcAmp = 0;
		//timing variables
		unsigned long prevUpdate;
		int sweepTime, updateFreq;
		unsigned int determineTime;

		//Position variables
		long startPos, endPos, currPos;
		int tick, tickCount;

		//flags
		volatile boolean arrived;
		boolean begun = false;
		boolean arc = false;

		//servo constants
		int SERVO_INDEX;
		int SERVO_MIN;
		int SERVO_MAX;

};


#endif
