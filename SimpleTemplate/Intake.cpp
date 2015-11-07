#include "Intake.h"
#include "Endgame.h"
#include "MyRobot.h"

IntakeClass::IntakeClass()
{
	Intake = new Victor(3); 
	Uptake = new Victor(4); 
	RampUp = new Solenoid(5);
	RampDown = new Solenoid(6);
	IsIntakeOn = 0;
	IsOuttakeOn = 0;
	UptakeStatus = "";
	UptakeSpeed = .8f;//.7 old bot
	RampStatus = -1;
}
void IntakeClass::IntakeOn()
{
	Uptake->Set(-UptakeSpeed);
	Intake->Set(-1.0f);
	//RampUp->Set(false);
	  //  RampDown->Set(true);
	RampStatus = -1;
}
void IntakeClass:: IntakeOff()
{
	Uptake->Set(0.0f);
	Intake->Set(0.0f);
	//RampUp->Set(true);
   // RampDown->Set(false);
	RampStatus = 1;
}
void IntakeClass::UpdateIntake(int OUTBTN, bool INTRIG, int RAMPBTN)//leftstickgetbtn2, leftstickgetgrig
{
	if((OUTBTN == 0) && (INTRIG == false))
	{
		Uptake->Set(0.0f);
		Intake->Set(0.0f);
		IsIntakeOn = 0;
		IsOuttakeOn = 0;
		UptakeStatus = "Off";
		RampStatus = -1;
	}
	else if((OUTBTN == 1) && (INTRIG == true))
	{
		Uptake->Set(0.0f);
		Intake->Set(0.0f);
		IsIntakeOn = 0;
		IsOuttakeOn = 0;
		UptakeStatus = "Off";
		RampStatus = -1;
		
	}
	else if ((OUTBTN == 0) && (INTRIG == true))
	{
		Uptake->Set(-UptakeSpeed);
		Intake->Set(-1.0f);
		IsIntakeOn = 1;
		IsOuttakeOn = 0;
		UptakeStatus = "Intake";
		RobotDemo::Get()->ARDUINO_SET(1,1,0);
		RampStatus = 1;
	}
	else if ((OUTBTN == 1) && (INTRIG == false))
	{
		Uptake->Set(UptakeSpeed);
		Intake->Set(1.0f);
		IsIntakeOn = 0;
		IsOuttakeOn = 1;
		UptakeStatus = "Outtake";
		RobotDemo::Get()->ARDUINO_SET(1,1,0);
		RampStatus = 1;
	}
	
	if(RampStatus == 1)
	{
		RampUp->Set(true);
		RampDown->Set(false);
	}
	else if (RampStatus == -1)
	{
		RampUp->Set(false);
		RampDown->Set(true);
	}
}
void IntakeClass::UpdateRampInput(int RAMPBTN)
{
	if(RobotDemo::Get()->GameTimer->Get()>=119.0f)
	{
		RampStatus = -1;
	}
	else
	{
		if(RobotDemo::Get()->Endgame->webehangin == 1)
		{
			RampStatus = -1;
		}
		else
	{
		if(RAMPBTN == 1)
			{
				RampStatus = -1;
		
			}
			else
			{
				RampStatus = 1;
			}
		}
	}
	
}
void IntakeClass::SendData()
{
	//SmartDashboard *Dash = SmartDashboard::GetInstance();
	//Dash->PutString("Uptake",UptakeStatus);
}

