
#include "DriveTrain.h"
#include "Ace_Defines.h"
#include <math.h>

DriveTrainClass::DriveTrainClass()
{
	LeftSideTicks = 0;
	RightSideTicks = 0;
	
	PrevLEnc = 0;
	PrevREnc = 0;
	
	
	LeftDrive = new Talon(Tal_Drive_Left); 
	RightDrive = new Talon(Tal_Drive_Right);

	LeftDriveEnc = new Encoder(1,2);
	RightDriveEnc = new Encoder(3,4);
	
	ShifterHigh = new HRSolenoid(1,1);
	ShifterLow = new HRSolenoid(1,2);

	
	
	CurrentShifterToggleTrig = false;
	PrevShifterToggleTrig = false;
	ToggleState = 1;
	Highgear = false;
	Lowgear = false;

	
}


void DriveTrainClass::Drive_Auton(float Forward, float Turn)
{			
	LeftDrive->Set(-Forward-Turn);
	RightDrive->Set(Forward-Turn);
}
void DriveTrainClass::Drive_Operator(float lY, float rY, int Ebrake)
{
	LeftDrive->Set(-lY);
	RightDrive->Set(rY);	
}
void DriveTrainClass::ResetEncoders_Timers()
{
	RightDriveEnc->Reset();
	RightDriveEnc->Start();
	LeftDriveEnc->Reset();
	LeftDriveEnc->Start();
}
void DriveTrainClass::Shifter_Update(bool ShifterEnable)//<-checks for a trigger "click"
{
	PrevShifterToggleTrig = CurrentShifterToggleTrig;
	CurrentShifterToggleTrig = ShifterEnable;
	
	if(PrevShifterToggleTrig == false && CurrentShifterToggleTrig == true)//<-checks for a trigger "click"
	{
		ToggleState = -ToggleState;
	}
	else if(ToggleState == 1)
	{
		ShifterHigh->Set(true);
		ShifterLow->Set(false);
	}
	else if(ToggleState == -1)
	{
		ShifterHigh->Set(false);
		ShifterLow->Set(true);
	}
	if((ShifterHigh->Get() == false)&&(ShifterLow->Get() == true))
	{
		Lowgear = true;
		Highgear = false;
	}
	if((ShifterHigh->Get() == true)&&(ShifterLow->Get() == false))
	{
		Lowgear = false;
		Highgear = true;
	}
}
void DriveTrainClass::SendData()
{
	SmartDashboard::PutNumber("Left Drive Encocder",(int)(LeftDriveEnc->Get()));
	SmartDashboard::PutNumber("Right Drive Encocder",(int)(RightDriveEnc->Get()));

	SmartDashboard::PutNumber("Left Drive Command",(double)LeftDrive->Get());
	SmartDashboard::PutNumber("Right Drive Command",(double)RightDrive->Get());
}
