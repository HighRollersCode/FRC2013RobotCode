
#include "DriveTrain.h"
#include "Ace_Defines.h"
#include "HRLogger.h"
#include <math.h>
DriveTrainClass::DriveTrainClass()
{
	LeftSideTicks = 0;
	RightSideTicks = 0;
	EncoderFailCount = 0;
	TickingEncoders = 1;
	PrevLEnc = 0;
	PrevREnc = 0;
	current_AutonTicks = 0;
	
	LeftDrive = new Talon(Tal_Drive_Left); 
	RightDrive = new Talon(Tal_Drive_Right);
	//RightDrive_a = new Talon(2);//2
	//LeftDrive_a = new Talon(4);//4
	LeftDriveEnc = new Encoder(1,2);
	RightDriveEnc = new Encoder(3,4);
	
	ShifterHigh = new Solenoid(1,1);
	ShifterLow = new Solenoid(1,2);

	CurrentBrakeBTN = 0;
	PrevBrakeBTN = 0;
	
	CurrentShifterToggleTrig = false;
	PrevShifterToggleTrig = false;
	ToggleState = 1;
	Highgear = false;
	Lowgear = false;

	HRLogger::Log("drive created\n");
	Ebrakemult = .1f;
}

void DriveTrainClass::UpdateEBrake(int enable,int targ)
{
	PrevBrakeBTN = CurrentBrakeBTN;
	CurrentBrakeBTN = enable;
	if((CurrentBrakeBTN == 1)&&(PrevBrakeBTN == 0))
	{
		LeftDriveEnc->Reset();
		RightDriveEnc->Reset();
		LeftDriveEnc->Start();
		RightDriveEnc->Start();
	}
	if(enable == 1)
	{
		LeftSideTicks = LeftDriveEnc->Get();

		//RightSideTicks = (int)(RightDriveEnc->Get()*2.5f);
		RightSideTicks = RightDriveEnc->Get();
		float Lerror = LeftSideTicks-targ;
		float Rerror = RightSideTicks+targ;
	
		float Leftout = -Lerror*Ebrakemult;
		float Rightout = -Rerror*Ebrakemult;
		
		LeftDrive->Set(Leftout);
	//	LeftDrive_a->Set(Leftout);		
		RightDrive->Set(Rightout);
	//	RightDrive_a->Set(-Rightout);
	}
	if((CurrentBrakeBTN == 0) && (PrevBrakeBTN == 1))
	{
		LeftDriveEnc->Reset();
		RightDriveEnc->Reset();
		LeftDriveEnc->Stop();
		RightDriveEnc->Stop();
	}
}
void DriveTrainClass::Drive_Auton(float Forward, float Turn)
{			
	LeftDrive->Set(-Forward-Turn);
	RightDrive->Set(Forward-Turn);
	
//	LeftDrive_a->Set(-Forward-Turn);
//	RightDrive_a->Set(Forward-Turn);
}
void DriveTrainClass::Drive_Operator(float lY, float rY, int Ebrake)
{
	if(Ebrake == 0)
	{
	LeftDrive->Set(-lY);//66
//	LeftDrive_a->Set(-lY);
	RightDrive->Set(rY);
//	RightDrive_a->Set(rY);
	}
	
}
void DriveTrainClass::ResetEncoders_Timers()
{
	RightDriveEnc->Reset();
	RightDriveEnc->Start();
	LeftDriveEnc->Reset();
	LeftDriveEnc->Start();
}
void DriveTrainClass::Shifter_Update(bool ShifterEnable)//rightstickgettrig,rightstickbtn10
{
	PrevShifterToggleTrig = CurrentShifterToggleTrig;
	CurrentShifterToggleTrig = ShifterEnable;
	
	if(PrevShifterToggleTrig == false && CurrentShifterToggleTrig == true)
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

void DriveTrainClass::Failsafe_Update()
{
	int Ldelta = LeftDriveEnc->Get() - PrevLEnc;
	int Rdelta = RightDriveEnc->Get() - PrevREnc;
	PrevLEnc = LeftDriveEnc->Get();
	PrevREnc = RightDriveEnc->Get();
	if(fabs(LeftDrive->Get()) > .5f)
	{
		if(Ldelta == 0)
		{
			EncoderFailCount++;
		}
	}
	if(fabs(RightDrive->Get()) > .5f)
	{
		if(Rdelta == 0)
		{
			EncoderFailCount++;
		}
	}
	if(EncoderFailCount > 1500)
	{
		TickingEncoders = 0;
	}
}
void DriveTrainClass::Failsafe_Reset()
{
	EncoderFailCount = 0;
	TickingEncoders = 1;
	PrevLEnc = LeftDriveEnc->Get();
	PrevREnc = RightDriveEnc->Get();
}
