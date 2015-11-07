#ifndef INTAKE_H
#define INTAKE_H

#include "WPIlib.h"



class IntakeClass
{
	Victor *Intake;
	Victor *Uptake;
public:
	Solenoid *RampUp;
	Solenoid *RampDown;
	string UptakeStatus;
	float UptakeSpeed;
		
	int IsIntakeOn;
	int IsOuttakeOn;
	int RampStatus;
	IntakeClass();
	~IntakeClass();
	
	void IntakeOn();
	void IntakeOff();
	void UpdateIntake(int OUTBTN, bool INTRIG, int RAMPBTN);
	void UpdateRampInput(int RAMPBTN);
	void SendData();
};
#endif
