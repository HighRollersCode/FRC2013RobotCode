#include "Turret.h"
#include "HRLogger.h"
#include "math.h"

//float CurrTiltInput = 0;
//float PrevTiltInput = 0;
//int TargetTiltEnc = -30;
//int CurrentTiltEnc = 0;
//practbot:
//static float AngleTable[] = { 33.9, 	29.5, 	26.8, 		23.0,	19.5,	18.3,	16.9,	15.3,	13.5,	12.2,	10.0,	9.3,	8.6,	7.2};
//static int EncoderTable[] = { 0,   	64,   83,    	108,   130,	136,	147,	161,	176,	184,	204,	216,	226,	243};
//static float DegToTickSlope = -9.6361;
//static float DegToTickIntercept = 306.4;
//291


//newstuff
static float AngleTable[] = { 12.1, 12.4, 13.2, 16.4,	19.4,	20.5,	23.8,	25.6,	27.7,	30.7,	31.4,	33.2};
static int EncoderTable[] = { 0,   -12,   -16,    -34,  -48,	-55,	-68,	-77,	-87,	-103,	-107,	-121};

static float DegToTickSlope = -5.0241;
static float DegToTickIntercept = 49.966;

static float TickToDegSlope = 1.0f/DegToTickSlope;
static float TickToDegIntercept = -DegToTickIntercept/DegToTickSlope;

TurretClass::TurretClass()
{
	currentTurretStickX = 0;
	previousTurretStickX = 0;
	turretcommand = 0;
	currentTurretPos = 0;
	targetTurretPos = 0;
	current_ClickIndex = 0;
	previous_ClickIndex = 0;
	currentEnableAuto = 0;
	previousEnableAuto = 0;
	currentholdgyro = 0;
	prevholdgyro = 0;
	ClickIndexNew = 0;

	TargetX = 0;   
	ClickNumber = 0;
	AngleX = 0;
	TargetY = 0;
	AngleY = 0;
	Auto_AngleY = 0;
	Auto_AngleX = 0;
	 
	Turret = new Victor(5);
	Tilt = new Victor(6);
	TurretGyro = new Gyro(1,1);
	TurretGyro->SetSensitivity(.007);
	TiltEnc = new Encoder(7,8);
	TurretEnc = new Encoder(5,6);
	ResetTimer = new Timer();
	CurrTiltInput = 0;
	PrevTiltInput = 0;
	TargetTiltEnc = 0;
	CurrentTiltEnc = 0;
	currentTurretEnc = 0;
	targetTurretEnc = 0;
	TiltMultiplier = 0.1f;
	TiltMin = 0.05f;
	PitchScale = .9f;
	
	CurrUp = 0;
	PrevUp = 0;
	CurrDown = 0;
	PrevDown = 0;
	
	TurretL_Limit = 0;
	TurretR_Limit = 0;
	TurretYawK = .02f;
	TurretYawMin = .05;
	
	CurrReset = 0;
	PrevReset = 0;
	maxtilt = -60;
	
	Ontarget = false;
	MaximumXError = 4.0f;
	MaximumYError = .1f;
}
TurretClass::~TurretClass(){}
void TurretClass::HoldTurret(int input)
{
	prevholdgyro = currentholdgyro;
	currentholdgyro = input;
	if((currentholdgyro == 1)&&(prevholdgyro == 0))
	{
		targetTurretPos = currentTurretPos;
	}
	if(currentholdgyro == 1)
	{
		currentTurretPos = TurretGyro->GetAngle();
		float error = currentTurretPos - targetTurretPos;
		float Mult = -.03f;
		float motorOut = error*Mult;
		Turret->Set(motorOut);
	}
}
void TurretClass::SetTarg(float v)
{
	targetTurretPos = v + TurretGyro->GetAngle();
}
void TurretClass::SetTargABS(float v)
{
	targetTurretEnc = 0;
	targetTurretPos = v;
}
void TurretClass::UpdateTurret(float Input,int lock,int enableauto)
{
	previousTurretStickX = currentTurretStickX;
	currentTurretStickX = Input;
	
	previous_ClickIndex = current_ClickIndex;
	current_ClickIndex = ClickNumber; 

	currentTurretPos = TurretGyro->GetAngle();
	currentTurretEnc = TurretEnc->Get();
	previousEnableAuto = currentEnableAuto;
	currentEnableAuto = enableauto;

	if(current_ClickIndex > previous_ClickIndex)
	{
		//if(ClickIndexNew > 2)
		//{
		targetTurretPos = (float)(currentTurretPos)+(AngleX);
	//	}
	//	ClickIndexNew++;
	}//	
	if(fabs((double)currentTurretStickX) >= .2f)
	{
		if(fabs((double)previousTurretStickX) <= .2f)
		{
			targetTurretPos = 0;
			targetTurretEnc = 0;
		}
	}
	if(fabs((double)currentTurretStickX) <= .09f)
		{
			if(fabs((double)previousTurretStickX) >= .09f)
			{
				
				targetTurretEnc = currentTurretEnc;
			}
		}
	if(fabs((double)currentTurretStickX) <= .05f)
	{
		currentTurretStickX = 0.0f;
		
	}
	if((currentEnableAuto == 1)&&(previousEnableAuto == 0))
	{printf("EnableAutoAim\n");
		NetworkTable * tbl = NetworkTable::GetTable("987Table");
		if (tbl != NULL)
		{
			float yaw = Auto_AngleX;
			float pitch = Auto_AngleY;
			float currentangle = GetTiltDegrees(CurrentTiltEnc);
			float newangle = currentangle - pitch*PitchScale;
			printf("yaw:%f   pitch:%f   currangle:%f   newangle%f\n",yaw,pitch,currentangle,newangle);
			TargetTiltEnc = (int)GetTiltTicks(newangle);
			targetTurretEnc = 0;
			SetTarg(yaw);
		}
	}
	if((targetTurretPos == 0))
	{	
		if(targetTurretEnc == 0)
		{
		 turretcommand = currentTurretStickX;
		}
	}
	else
	{
		float error = targetTurretPos - currentTurretPos;
		
		float min = 0.0f;
		
		if(error > 0)
		{
			min = TurretYawMin;
		}
		else if (error < 0)
		{
			min = -TurretYawMin;
		}
		
		float motorout = (error*TurretYawK) + min;
		turretcommand = motorout;
	}
	if(TurretEnc->Get() <= TurretR_Limit)
	{
		if(Input > 0)
		{
			turretcommand = 0;
		}
	}
	if(TurretEnc->Get() >= TurretL_Limit)
	{
		if(Input < 0)
		{
			turretcommand = 0;
		}
	}	
	if(targetTurretEnc != 0)
	{
		float errorenc = targetTurretEnc - currentTurretEnc;
		turretcommand = -errorenc * .015f;
	}
	Turret->Set(turretcommand);
}
void TurretClass::Reset_Sensors()
{
	TurretGyro->Reset();
	TiltEnc->Start();
	TiltEnc->Reset();
	TurretEnc->Start();
	TurretEnc->Reset();
}
void TurretClass::SendData()
{
	SmartDashboard::PutNumber("Gyro",(double)TurretGyro->GetAngle());
	SmartDashboard::PutNumber("TurrCom", (double)turretcommand);
	SmartDashboard::PutNumber("TurrTarget",(double)targetTurretPos);
	SmartDashboard::PutNumber("TiltEncoder",(int)TiltEnc->Get());
	SmartDashboard::PutNumber("TurretEncoder",(int)TurretEnc->Get());
	SmartDashboard::PutNumber("TiltTarget",(int)TargetTiltEnc);
	//SmartDashboard::PutNumber("Leftlim",(int)TurretL_Limit);
	//SmartDashboard::PutNumber("Rightlim",(int)TurretR_Limit);
	SmartDashboard::PutNumber("Click_Pos",TargetX);
	SmartDashboard::PutNumber("Angle_Calc",AngleX);
	SmartDashboard::PutNumber("Click_Index",ClickNumber);
	
}
void TurretClass::GetData()
{
	try
	{
		//lprintf("beforeTBLinit\n");
		NetworkTable * tbl = NetworkTable::GetTable("987Table");
		//printf("afterTBLinit\n");
		//	AngleX = 0;
		//	TargetX = 0;
		//	AngleY = 0;
		//	TargetY = 0;
		//	Auto_AngleX =0;
		//	Auto_AngleY = 0;
		//printf("BEFORETABLECHEC\n");
		if (tbl != NULL)
		{
		//	printf("INSIDETABLECHECK\n");
			//tbl->BeginTransaction();
			//tbl->GetNumber("987Seq");
			///printf("1\n");
			ClickNumber = (int)tbl->GetNumber("987Seq");
			//printf("2\n");
	
			AngleX = ((float)tbl->GetNumber("987Yaw"))/1000;
		//	printf("3\n");
			TargetX = ((float)tbl->GetNumber("987ClickX"))/1000;
		//	printf("4\n");
			AngleY = ((float)tbl->GetNumber("987Pitch"))/1000;
		//	printf("5\n");
			TargetY = ((float)tbl->GetNumber("987ClickY"))/1000;
		//	printf("%f\n",TargetY);
		//	printf("6\n");
			float as = ((float)tbl->GetNumber("987AutoSeq"));
			float acx = ((float)tbl->GetNumber("987AutoClickX"));
			Auto_AngleX = ((float)tbl->GetNumber("987AutoYaw"))/1000;
			float acy = ((float)tbl->GetNumber("987AutoClickY"));
		//	printf("%\n",Auto_AngleX);
		//	printf("7\n");
			Auto_AngleY = ((float)tbl->GetNumber("987AutoPitch"))/1000;
		//	printf("8\n");
			
			if(fabs(Auto_AngleX) <= MaximumXError)
			{
				if(fabs(Auto_AngleY) <=MaximumYError)
				{
					Ontarget = true;
				}
			}
			else{Ontarget = false;}
			
				//tbl->EndTransaction();
		}
		else
		{
			ClickNumber = 0;
			AngleX = 0.0f;
			TargetX = 0.0f;
			AngleY  = 0.0f;
			TargetY  = 0.0f;
			Auto_AngleX = 0.0f;
			Auto_AngleY = 0.0f;
			Ontarget = false;
		}

	}
	catch(exception)
	{
		ClickNumber = 0;
		AngleX = 0.0f;
		TargetX = 0.0f;
		AngleY  = 0.0f;
		TargetY  = 0.0f;
		Auto_AngleX = 0.0f;
		Auto_AngleY = 0.0f;
		Ontarget = false;
	}
}
void TurretClass::UpdateTilt(float Input,int Down, int Up,int enableauto)
{
	PrevTiltInput = CurrTiltInput;
	CurrTiltInput = Input;
	PrevUp = CurrUp;
	CurrUp = Up;
	PrevDown = CurrDown;
	CurrDown = Down;
	
	CurrentTiltEnc = TiltEnc->Get();
	float OUT;	
	int Error;
	
	if(current_ClickIndex > previous_ClickIndex)
	{
		float currentangle = GetTiltDegrees(CurrentTiltEnc);
		float newangle = currentangle - AngleY*PitchScale;
		TargetTiltEnc = (int)GetTiltTicks(newangle);
	}
	if((CurrDown == 1) ||(CurrUp == 1))
	{
		TargetTiltEnc = 0;
	}
	if(TargetTiltEnc == 0)
	{
		if(CurrDown == 1)
		{
			OUT = .2f;
			TargetTiltEnc = 0;
		}
		if(CurrUp == 1)
		{
			if(TiltEnc->Get() < maxtilt)
			{
				OUT = 0;
				
			}
			else
			{
				OUT = -.5f;
				TargetTiltEnc = 0;
			}
		}
	}
	else
	{
		Error = TargetTiltEnc - CurrentTiltEnc;
	
		float min = 0.0f;
		
		if(Error > 0)
		{
			min = TiltMin;
		}
		else if (Error < 0)
		{
			min = -TiltMin;
		}
		
		OUT = Error*TiltMultiplier+min;
	}
	if((CurrUp == 0)&&(PrevUp == 1))
	{
		TargetTiltEnc = CurrentTiltEnc;
	}
	if((CurrDown == 0)&&(PrevDown == 1))
	{
		TargetTiltEnc = CurrentTiltEnc;
	}

	Tilt->Set(OUT);
	SmartDashboard::PutNumber("TiltCom",(double)OUT);
	SmartDashboard::PutNumber("CTiltInput",(double)CurrTiltInput);
	SmartDashboard::PutNumber("PTiltInput",(double)PrevTiltInput);
}
void TurretClass::UpdateReset(int Input, int turretenc, int tiltenc)
{
	CurrReset = Input;
	if((CurrReset == 1))
	{
		targetTurretEnc = turretenc;
		TargetTiltEnc = tiltenc;
	//	targetTurretEnc = -264;
	//	TargetTiltEnc = -41;
	}
}
float TurretClass::GetTiltTicks(float Degrees)
{
	return (Degrees * DegToTickSlope) + DegToTickIntercept;
}
float TurretClass::GetTiltDegrees(float Ticks) 
{
	return (Ticks * TickToDegSlope) + TickToDegIntercept;
}
