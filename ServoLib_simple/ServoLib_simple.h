

#ifndef ServoLib_simple_h
#define ServoLib_simple_h

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <avr/dtostrf.h>


class ServoLib_simple
{
	public:
		ServoLib_simple(int _SERVO_INDEX, int _SERVO_MIN, int _SERVO_MAX );

		void begin(int _startingPos, int _PWMFreq); //60 Hz => deg 90 = pulse 350; 120 Hz => deg 90 = pulse 700
		int read();
		void write(int _servoTarget);
		void write(int _servoTarget, int _sweepTime, int _updateFreq);

		int isRunning();
		void debugEaser();

	private:
		//external libraries
		Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
		//internal interrupt methods
		void interrupt_checkNewPos(int _servoTarget);
		void update_sweep();
		void update_simple();
		int err(int errVal);
		int getNewpos();

		//timing variables
		unsigned long prevUpdate;
		int sweepTime, updateFreq;
		unsigned int determineTime;

		//Position variables
		int startPos, endPos, currPos;
		int tick, tickCount;

		//flags
		volatile boolean arrived;
		boolean begun = false;

		//servo constants
		int SERVO_INDEX;
		int SERVO_MIN;
		int SERVO_MAX;

};


#endif
