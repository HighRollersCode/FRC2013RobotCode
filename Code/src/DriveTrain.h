#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "WPILib.h"

class DriveTrainClass
{
	int current_AutonTicks;
	int EncoderFailCount;

	int PrevLEnc;
	int PrevREnc;
	Talon *LeftDrive;
///	Victor *LeftDrive_a;
	Talon *RightDrive;
///	Victor *RightDrive_a;
	
	Encoder *LeftDriveEnc;
	Encoder *RightDriveEnc;
	
	Solenoid *ShifterHigh;
	Solenoid *ShifterLow;
	
	int CurrentBrakeBTN;
	int PrevBrakeBTN;
	bool CurrentShifterToggleTrig;
	bool PrevShifterToggleTrig;
	
public:	
	int TickingEncoders;
	int LeftSideTicks;
	int RightSideTicks;
	int ToggleState;
	bool Highgear;
	bool Lowgear;
	float Ebrakemult;
	
	DriveTrainClass();
	
	~DriveTrainClass();
	void UpdateEBrake(int enable, int targ);
	void Drive_Auton(float Forward, float Turn);
	
	void Drive_Operator(float lY, float rY, int Ebrake);
	void ResetEncoders_Timers();
	int GetLeftEncoder(){return LeftDriveEnc->Get();}
	int GetRightEncoder(){return RightDriveEnc->Get();}
	
	void Shifter_Update(bool ShifterEnable);
	void SendData();	
	
	void Failsafe_Update();
	void Failsafe_Reset();
};
#endif
