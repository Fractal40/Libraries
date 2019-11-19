//
// InverseKinematics.h -- Arduino library for calculating inverse kinematics for
// a 3 DOF crawler leg.
//
//
// Features:
// - Supports both angles in degrees and pulsewidth (for use with Adafruit_PWMServoDriver
//
// 2019, Lars Ström
//
//


#ifndef InverseKinematics_h
#define InverseKinematics_h

#include "Arduino.h"
#include <avr/dtostrf.h>



class InverseKinematics
{
	public:
		InverseKinematics(int _limbA_Len, int _limbB_Len, int _limbC_Len);
    void ik(int _x, int _y, int _z);
    int getAngleDeg(int _jointNum);
    int getAnglePulse(int _jointNum);

	private:

    int limbA_Len, limbB_Len, limbC_Len;
    int joint[3];
		int x, y, z;
		int startAngle1, startAngle2, startAngle3;
		String inputMethod;


};


#endif
