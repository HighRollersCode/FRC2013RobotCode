#include "ShooterWheel.h"

#define Shooter_Preset_Low 5200.0f
//#define Shooter_Preset_Med 2500.0f
//#define Shooter_Preset_High 4000.0f

static float rpEr;
static float rpDes;
static float cmdCorrection;
//
// RPM -> Motor Command Table
//
static float RPMTable[] = { 1475, 3610.0, 4675.0, 5075.0, 5380.0,6000.0};
static float MotorCmd[] = { 0.127,    1.0*.8f,   1.0*.8f,   1.0*.85,   1.0*.85f, 1.0};


//static int RPM_TABLE_COUNT = sizeof(NEWRPMBANK) / sizeof(NEWRPMBANK[0]);

//what we had for 2013 very recently
static float NEWDIST[] = {8.8f, 10.75f};
static float NEWRPMBANK[] = {1538.0, 1690.0f};
//static int RPM_TABLE_COUNT = sizeof(NEWRPMBANK) / sizeof(NEWRPMBANK[0]);
static int RPM_TABLE_COUNT = sizeof(RPMTable) / sizeof(RPMTable[0]);
static double currenttime = 0.0;
static double previoustime = 0.0;
double ShooterWheelClass::Shooter_WheelK = .03f;
ShooterWheelClass::ShooterWheelClass()
{
	Shooter = new Victor(7);
	
	GearSensor = new GearTooth(1,9,false);
	GearSensor->Reset();
	GearSensor->Start();
	
	ShooterToggle = 1; 
	State = ShooterState_off;
	
	currentPresetSpeed = 0.0f;
	OverrideCommand = 0.0f;
	
	RPMCOUNT = 0.0;
	PREVRPMCOUNT = 0.0;
	RPM = 0.0;
	DTIME = 0.0;
	DCOUNT = 0.0;
	INDICATOR = 0;

}
void ShooterWheelClass::SetSpeed(float command)
{
	Shooter->Set(command);
}

float ShooterWheelClass::EstimatePower(float desiredRPM)
{
	if(desiredRPM < RPMTable[0]){return MotorCmd[0];}
	if(desiredRPM > RPMTable[RPM_TABLE_COUNT-1]){return MotorCmd[RPM_TABLE_COUNT-1];}
	
	for(int i = 0; i < RPM_TABLE_COUNT; i++)
	{
		if(desiredRPM < RPMTable[i])
		{
			float norm = (desiredRPM - RPMTable[i-1])/(RPMTable[i] - RPMTable[i-1]);
			float command = (MotorCmd[i-1] + norm*(MotorCmd[i]-MotorCmd[i-1]));
			return command;
		}
	}
	return MotorCmd[RPM_TABLE_COUNT-1];
}
float ShooterWheelClass::PUpdate(float desrpm)
{
	
	double basecom = EstimatePower(desrpm);
	double error = desrpm - RPM;
//	double mult = .03;
	double correction = (error*Shooter_WheelK);
	if (correction > 1.0f) 
	{
		correction = 1.0f;
	}
	if (correction < -0.1f)//.2 
	{
		correction = -0.1f;
	}
	double command = correction + basecom;
	if (command > 1.0)
	{
		command = 1.0;
	}
	
	cmdCorrection = correction;
	rpDes = desrpm;
	rpEr = error;
	return -command;
	
}
void ShooterWheelClass::ShooterOverride(float input)
{
	
	float normval = (input+1.0f)/2.0f;
	
	float MIN_COM = 0.0f;
	float MAX_COM = 1.0f;
	
	//float MIN_COM = 1400.0f;
	//float MAX_COM = 1900.0f;
	OverrideCommand = -(MIN_COM+normval*(MAX_COM-MIN_COM));
}
void ShooterWheelClass::ShooterOverrideRPM(float rpm)
{
	OverrideCommand = rpm;
}
void ShooterWheelClass::WheelOff()
{
	Shooter->Set(0.0f);
	ShooterToggle = 1;
}
/*
void ShooterWheelClass::UpdateShooter(
		int EnableLow,
		int EnableMed,
		int EnableHigh,
		int EnableOverride,
		double RobotTime)//turretstickrawbutton3
{
	RPMCOUNT = GearSensor->Get();
	currenttime = RobotTime;
	double dt = currenttime-previoustime;

	if((dt > 0.01f) || (dt < 0.0f))
	{
		DTIME = dt;
		DCOUNT = (RPMCOUNT-PREVRPMCOUNT);
		RPM = ((1.0/(GearSensor->GetPeriod()))*60.0)/6;
		previoustime = currenttime;
		PREVRPMCOUNT = RPMCOUNT;
	}
	if((EnableLow == 1)&&(prevlow == 0))
	{
		SetState(ShooterState_low);
		currentPresetSpeed = Shooter_Preset_Low;
	}
	if((EnableMed == 1)&&(prevmed == 0))
	{
		SetState(ShooterState_med);
		currentPresetSpeed = Shooter_Preset_Med;
	}
	if((EnableHigh == 1)&&(prevhigh == 0))
	{
		SetState(ShooterState_high);
		currentPresetSpeed = Shooter_Preset_High;
	}
	if((EnableOverride == 1)&&(prevoverride == 0))
	{
		SetState(ShooterState_overrideRPM);
	}
	
	if(ShooterToggle == 1)
	{
		SetSpeed(0.0f);//-.78
		INDICATOR = 0;
	}
	else if(ShooterToggle == -1)
	{
		INDICATOR = 1;

	    if(State == ShooterState_overrideRPM)
		{
			currentPresetSpeed = (OverrideCommand);
		}
		else
		{			
		}
		
	    if(State == ShooterState_override)
		{
			SetSpeed(OverrideCommand); // currentPresetSpeed);
		}
		else
		{
			SetSpeed(PUpdate(currentPresetSpeed));
		}
		//SetSpeed(currentPresetSpeed);
		//SetSpeed(PUpdate(5.0f));
	}
	else
	{
		SetSpeed(0.0f);
	}
	//Shooter2->Set(Shooter->Get());
	prevlow = EnableLow;
	prevhigh = EnableHigh;
	prevmed = EnableMed;
	prevoverride = EnableOverride;
}
*/
void ShooterWheelClass::UpdateShooter(
		int EnableLow,
		int EnableOverride,
		double RobotTime)//turretstickrawbutton3
{
	RPMCOUNT = GearSensor->Get();
	currenttime = RobotTime;
	double dt = currenttime-previoustime;

	if((dt > 0.01f) || (dt < 0.0f))
	{
		DTIME = dt;
		DCOUNT = (RPMCOUNT-PREVRPMCOUNT);
		RPM = ((1.0/(GearSensor->GetPeriod()))*60.0)/6;
		previoustime = currenttime;
		PREVRPMCOUNT = RPMCOUNT;
	}
	if((EnableLow == 1)&&(prevlow == 0))
	{
		SetState(ShooterState_low);
		currentPresetSpeed = Shooter_Preset_Low;
	}
	if((EnableOverride == 1)&&(prevoverride == 0))
	{
		SetState(ShooterState_overrideRPM);
		//SetState(ShooterState_override);
	}
	
	if(ShooterToggle == 1)
	{
		SetSpeed(0.0f);//-.78
		INDICATOR = 0;
	}
	else if(ShooterToggle == -1)
	{
		INDICATOR = 1;

	    if(State == ShooterState_overrideRPM)
		{
			currentPresetSpeed = (OverrideCommand);
		}
		else
		{			
		}
		
	    if(State == ShooterState_override)
		{
			SetSpeed(OverrideCommand); // currentPresetSpeed);
		}
		else
		{
			SetSpeed(PUpdate(currentPresetSpeed));
		}
	}
	else
	{
		SetSpeed(0.0f);
	}
	//Shooter2->Set(Shooter->Get());
	prevlow = EnableLow;
	prevoverride = EnableOverride;
}
void ShooterWheelClass::SetState(int newstate)
{
	ShooterToggle = -ShooterToggle;
	State = newstate;
}
void ShooterWheelClass::SendData()
{
	SmartDashboard::PutNumber("Shooter Command",Shooter->Get());
	SmartDashboard::PutNumber("G_T_Reading", GearSensor->Get());
	SmartDashboard::PutNumber("RPM",(double)RPM); //(1.0/GearSensor->GetPeriod())*60.0);	//RPM
	SmartDashboard::PutNumber("DesiredRPM",(double)rpDes);
	SmartDashboard::PutNumber("ErrorRPM",(double)rpEr);
	//SmartDashboard::PutNumber("SHOOTERMULT",(double)Shooter_WheelK);
	SmartDashboard::PutNumber("RPMCorr",(double)cmdCorrection);
}
