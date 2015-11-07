#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPIlib.h"
enum ShooterState
{
	ShooterState_off = 0,
	ShooterState_low,
	//ShooterState_med,
	//ShooterState_high,
	ShooterState_override,
	ShooterState_overrideRPM
};

class ShooterWheelClass
{	
	int prevlow;
	//int prevmed;
	//int prevhigh;
	int prevoverride;
	int State;
	float currentPresetSpeed;
	
	Victor *Shooter;
	GearTooth *GearSensor;
	double DTIME;
	double DCOUNT;
	
public:
	static double Shooter_WheelK;
	int INDICATOR;
	double RPMCOUNT;
	double RPM;
	double PREVRPMCOUNT;
	float OverrideCommand;
	int ShooterToggle; //1
	ShooterWheelClass();
	~ShooterWheelClass();
	
	void SetSpeed(float command);
	void WheelOff();
	//void UpdateShooter(int EnableLow,int EnableMed,int EnableHigh,
	//int EnableOverride,double RobotTime);
	void UpdateShooter(int EnableLow,int EnableOverride,double RobotTime);
	void ShooterOverride(float input);
	void ShooterOverrideRPM(float rpm);
	float EstimatePower(float desiredRPM);
	
	float PUpdate(float dist);
	//float USELINE(float dist);
	void SetState(int newstate);
	void SendData();
};
#endif
