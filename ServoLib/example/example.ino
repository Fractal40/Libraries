

#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>
#include <SerialStateMachine.h>
#include <ServoLib.h>


int servoTarget=350;
int servoNum = 0;
const int CNTRL_NR_INPUT = 3;
const int CNTRL_TRGT_INPUT = 4;
const int COXA_NR = 0;
const int FEMUR_NR = 1;
const int TIBIA_NR = 2;
const int MIN = 150;
const int MAX = 500;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
SerialStateMachine ssm = SerialStateMachine();
ServoLib servo = ServoLib(COXA_NR, MIN, MAX); //servo index number, servo min. position, servo max position

void setup()
{
	Serial.begin(38400); // Initialize Serial Monitor USB
	Serial1.begin(38400); // Initialize hardware serial port, pins 0/1

	pwm.begin();


	delay(1000);
	pwm.setPWMFreq(60);

  servo.begin(350); //starting value
  servo.setEasingFunc("easeInOut"); //easeInOut, easeOut, noEasing
  servo.setTiming(2000, 20) //traveltime in ms, update frequency in ms
  delay(1000);

}

void loop()
{

	controllerData();

  servo.write(servoTarget);

  //servo.debugEaser();
  //ssm.debug();

}


int controllerData()
{
	//Get data from serial

  while (Serial1.available()) {
    ssm.processIncomingByte(Serial1.read());
    servoTarget = ssm.getValue(CNTRL_TRGT_INPUT);
    servoNum = ssm.getValue(CNTRL_NR_INPUT);
    //ssm.debug();
  }



}
