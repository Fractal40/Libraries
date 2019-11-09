
#include "Arduino.h"
#include "InverseKinematics.h"
#include <avr/dtostrf.h>

InverseKinematics::InverseKinematics(int _limbA_Len, int _limbB_Len, int _limbC_Len)
{
  limbA_Len = _limbA_Len; //coxa length
  limbB_Len = _limbB_Len;  //femur length
  limbC_Len = _limbC_Len;  //tibia length
}

void InverseKinematics::ik(int _x, int _y, int _z)
{
  float legLen,d,b1,b2;

  x = _x; //limbA_Len + limbB_Len + _x;
  y = _y; //limbC_Len + _y;
  z = _z; //0 + _z;

  legLen = sqrt(x*x+z*z);
  joint[0] = atan2(z,x)*180/3,14;                                                                 //calculate angle 1
  d= sqrt(pow((legLen-limbA_Len),2)+(y*y));
  b1 = atan2((legLen-limbA_Len),(y))*180/3.14;
  b2 = acos((pow(d,2)+pow(limbB_Len,2)-pow(limbC_Len,2))/(2*d*limbB_Len))*180/3.14;
  joint[1] = (b1+b2)-90;                                                                          //calculate angle 2
  joint[2] = acos((-pow(d,2)+pow(limbC_Len,2)+pow(limbB_Len,2))/(2*limbB_Len*limbC_Len))*180/3.14;    //calculate angle 3

  joint[2] = joint[2];       //Adjust for servo orientations
  joint[1] = joint[1]+90;
  joint[0] = joint[0]+90;

}

int InverseKinematics::getAngleDeg(int _joint)
{
  return joint[_joint];
}

int InverseKinematics::getAnglePulse(int _joint)
{
  //If both getAngleDeg and getAnglePulse is to be used, getAnglePulse can only
  //be be used after getAngleDeg.
  joint[_joint] = map(joint[_joint],45,135,175,525); //convert and map degrees to pulsewidth
  return joint[_joint];
}
