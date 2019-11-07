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
    void degToPulse();
    int limbA_Len, limbB_Len, limbC_Len;
    int joint[3];
		int x, y, z;

};


#endif
