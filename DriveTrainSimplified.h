#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "WPILib.h"
#include "HRSolenoid.h"

class DriveTrainClass
{	
	int PrevLEnc;
	int PrevREnc;
	Talon *LeftDrive;
	Talon *RightDrive;
	
	Encoder *LeftDriveEnc;
	Encoder *RightDriveEnc;
	
	HRSolenoid *ShifterHigh;
	HRSolenoid *ShifterLow;
	
	
	bool CurrentShifterToggleTrig;
	bool PrevShifterToggleTrig;
	
public:	
	
	int LeftSideTicks;
	int RightSideTicks;
	int ToggleState;
	bool Highgear;
	bool Lowgear;
	
	
	DriveTrainClass();
	
	~DriveTrainClass();
	void Drive_Auton(float Forward, float Turn);
	
	void Drive_Operator(float lY, float rY, int Ebrake);
	void ResetEncoders_Timers();
	int GetLeftEncoder(){return LeftDriveEnc->Get();}
	int GetRightEncoder(){return RightDriveEnc->Get();}
	
	void Shifter_Update(bool ShifterEnable);
	void SendData();	
};
#endif
