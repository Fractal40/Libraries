
#include "Arduino.h"
#include "InverseKinematics.h"
#include <avr/dtostrf.h>

InverseKinematics::InverseKinematics(int _limbA_Len, int _limbB_Len, int _limbC_Len)
{
  limbA_Len = _limbA_Len; //coxa length
  limbB_Len = _limbB_Len  //femur length
  limbC_Len = _limbC_Len  //tibia length
}

void InverseKinematics::ik(int _x, int _y, int _z)
{
  float legLen,d,b1,b2;

  y = _y;
  x = _x;
  z = _z;

  legLen = sqrt(x*x+z*z);
  joint[0] = atan2(z,x)*180/3,14;                                                                 //calculate angle a
  d= sqrt(pow((legLen-limbA_Len),2)+(y*y));
  b1 = atan2((legLen-limbA_Len),(y))*180/3.14;
  b2 = acos((pow(d,2)+pow(limbB_Len,2)-pow(limbC_Len,2))/(2*d*limbB_Len))*180/3.14;
  joint[1] = (b1+b2)-90;                                                                          //calculate angle b
  joint[2] = acos((-pow(d,2)+pow(limbC_Len,2)+pow(limbB_Len,2))/(2*limbB_Len*limbC_Len))*180/3.14;    //calculate angle c

  joint[2] = joint[2];       //Adjust for servo orientations
  joint[1] = joint[1]+90;
  jointA[0] = joint[0]+90;

  //convDegToPulse();
}

int InverseKinematics::getAngleDeg(int _joint)
{
  return joint[_joint];
}

int InverseKinematics::getAnglePulse(int _joint)
{
  degToPulse();
  return joint[_joint];
}

void degToPulse()
{
  //map pulse on degrees for each joint
  joint[0] = map(joint[0],45,135,175,525);
  joint[1] = map(joint[1],45,135,175,525);
  joint[2] = map(joint[2],45,135,175,525);
}
