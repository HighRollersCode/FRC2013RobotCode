#include "Auton.h"
#include "HRLogger.h"
#include "math.h"
#include "stdio.h"


AutonomousController::AutonomousController
(
	DriveTrainClass *D,
	IntakeClass *I,
	LauncherClass *L,
	ShooterWheelClass *S,
	TurretClass *T,
	Watchdog *W,
	DriverStation *Ds
)
{
	
	DriveTrain = D;
	HRLogger::Log("autodrive created\r\n");
	Intake = I;
	Launcher = L;
	ShooterWheel = S;
	Turret = T;
	watchdog = W;
	ds = Ds;
	turningp = -.02f;
	AutonTimer = new Timer();
	SendTimer = new Timer();
	Abort = false;
}
void AutonomousController::Auto_Start()
{
//	log = fopen("autolog.txt","wa");
	watchdog->SetExpiration(.1);
	watchdog->SetEnabled(true);
	watchdog->Feed();
	AutonTimer->Reset();
	AutonTimer->Start();
	Launcher->ResetTimers();
	SendTimer->Reset();
	SendTimer->Start();
	Turret->targetTurretPos = 0.0f;
	Turret->TurretGyro->Reset();
	DriveTrain->ResetEncoders_Timers();
	DriveTrain->Failsafe_Reset();
	
	Auto_System_Update();
}
void AutonomousController::Auto_End()
{
	ShooterWheel->WheelOff();
}
bool AutonomousController::Running()
{
	if(Abort){return false;}
	else if (AutonTimer->Get() >= 15.0f){return false;}
	//else if (DriveTrain->TickingEncoders == 0){return false;}
	else if (ds->IsAutonomous() == false){return false;}
	else if (ds->IsEnabled() == false){return false;}
	return true;
}
void AutonomousController::AutonWait(float Seconds)
{
	float targ = AutonTimer->Get() + Seconds;
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		//printf("made it");
		Auto_System_Update();
	}
}
void AutonomousController::AutonWait2(float Seconds,int brake)
{
	float targ = AutonTimer->Get() + Seconds;
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
		DriveTrain->UpdateEBrake(1,brake);
	}
	DriveTrain->Drive_Auton(0.0f,0.0f);
}
void AutonomousController::Auto_DriveTimer(float Forward, float Turn, float Seconds)
{
	DriveTrain->Drive_Auton(Forward,Turn);
	AutonWait(Seconds);
	DriveTrain->Drive_Auton(0,0);
}
void AutonomousController::Auto_DriveEncoder(float Forward, float Turn, float Ticks)
{
	//printf("starting drive %p \r\n",DriveTrain);
	DriveTrain->ResetEncoders_Timers();
	//printf("Reset Encoders");
	if(Ticks > 0)
	{
	//	printf("Inside Driving");
		while((DriveTrain->GetLeftEncoder() < Ticks)&&(Running()))
		{
			DriveTrain->Drive_Auton(Forward, Turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	else
	{
		//printf("Inside Driving \r\n");
		while((DriveTrain->GetLeftEncoder() > Ticks)&&(Running()))
		{
			DriveTrain->Drive_Auton(Forward, Turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	printf("Finished Driving");
	DriveTrain->Drive_Auton(0,0);
//	DriveTrain->Failsafe_Reset();
}
void AutonomousController::Auto_GYROTURN(float heading)
{
	Turret->targetTurretPos = 0;
	DriveTrain->ResetEncoders_Timers();
	
	float GYRO_P = turningp;
	float angle_error = heading - Turret->currentTurretPos;
	float turn = GYRO_P * angle_error;
	while((fabs(angle_error) > 3.0f)&&(Running()))
	{
		angle_error = heading - Turret->currentTurretPos;
		turn = -(GYRO_P * angle_error);
		Auto_System_Update();
		DriveTrain->Drive_Auton(0,turn);
	}
}
void AutonomousController::Auto_GYROSTRAIGHT(float forward,float ticks,float desheading)
{
	DriveTrain->ResetEncoders_Timers();
	float MAINTAIN = desheading;
	float GYRO_P = -0.04f;
	if(forward < .6)
	{
		GYRO_P = -0.04f;
	}
	else
	{
		GYRO_P = -0.01f;
	}
	float angle_error = MAINTAIN - Turret->currentTurretPos;
	float turn = GYRO_P * angle_error;
	
	if(ticks > 0)
	{
		while((DriveTrain->GetLeftEncoder() < ticks)&&(Running()))
		{
			angle_error = MAINTAIN - Turret->currentTurretPos;
			turn = -(GYRO_P * angle_error);
			DriveTrain->Drive_Auton(forward, turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	else
	{
		while((DriveTrain->GetLeftEncoder() > ticks)&&(Running()))
		{
			angle_error = MAINTAIN - Turret->currentTurretPos;
			turn = -(GYRO_P * angle_error);
			DriveTrain->Drive_Auton(forward, turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	DriveTrain->Drive_Auton(0,0);
	//DriveTrain->Failsafe_Reset();
}
void AutonomousController::Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks)
{
	
	DriveTrain->ResetEncoders_Timers();
	Auto_GYROTURN(Angle);
	AutonWait(.7f);
	Auto_GYROSTRAIGHT(Forward,Ticks,Angle);
}

bool AutonomousController::Auto_System_Update()
{	
	watchdog->Feed();
	if(Running())
	{
		ShooterWheel->UpdateShooter(0,0,AutonTimer->Get());
		Turret->UpdateTurret(0.0f,0,0);
		Turret->UpdateTilt(0.0f,0,0,0);
		DriveTrain->Shifter_Update(0);
		Turret->GetData();
		Launcher->UpdateLaunch_Operator(0,0);
		Wait(.001f);

	}
	SendData();
	return true;

}
void AutonomousController::Auto_CameraAim()
{
	
	//float clickindex = Turret->current_ClickIndex;
	//float previndex = Turret->previous_ClickIndex;

	float yaw = (float)Turret->Auto_AngleX;
	float pitch = (float)Turret->Auto_AngleY;
	float currentangle = Turret->GetTiltDegrees(Turret->CurrentTiltEnc);
	float newangle = currentangle - pitch*Turret->PitchScale;
	Turret->TargetTiltEnc = (int)Turret->GetTiltTicks(newangle);
	Turret->SetTarg(yaw);
}
void AutonomousController::SendData()
{
	if(SendTimer->Get() > .05)
	{
		
		SendTimer->Reset();
		SendTimer->Start();
		try{
		DriveTrain->SendData();
		Turret->SendData();
		}
		catch(exception)
						{
							
						}
		
	}
}
/*
void AutonomousController::AUTO_Layup()// 4ball alliance bridge
{
//	AUTO_LOG((log,"startlayup\r\n"));
	ShooterWheel->SetState(ShooterState_overrideRPM);
	ShooterWheel->ShooterOverrideRPM(1538);// 1670.0f;
	DriveTrain->ToggleState = 1;
	ShooterWheel->ShooterToggle = -1;
	AutonWait(.25f);
//	AUTO_LOG((log,"test = %d\r\n",ShooterWheel->ShooterToggle));
	Auto_GYROSTRAIGHT(-1.0f,-220,0.0f);
	ShooterWheel->ShooterToggle = -1;
	//Auto_GYROSTRAIGHT(-.1f,-200,0.0f);
	//Auto_DriveTimer(.1f,0.0f,.025f);
	AutonWait2(.2f,-10);
	ShooterWheel->ShooterToggle = -1;
	DriveTrain->UpdateEBrake(0,0);
	Turret->SetTarg(-35.0f);
	AutonWait(1.0f);
	Auto_FullLaunch();
	Auto_IntakeOn();

	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
	}
	AutonWait(1.3f);
	Auto_FullLaunch();
	AutonWait(.2f);
	ShooterWheel->WheelOff();
	
	Turret->targetTurretPos = 0.0f;
	Turret->Reset_Sensors();
	autoindex = 1;
	Auto_LowerTomahawk();
	Auto_GYROSTRAIGHT(.35f,50,-40.0f);
	Auto_GYROSTRAIGHT(1.0f,250,-40.0f);
	Auto_GYROSTRAIGHT(.35f,230,-40.0f);
	
	Auto_GYROSTRAIGHT(.35f,50,0.0f);
	Auto_GYROSTRAIGHT(.8f,100,0.0f);
	Auto_GYROSTRAIGHT(.2f,540,0.0f);
	autoindex = 2;
	Auto_IntakeOn();
#if !PRACTICE_BOT
	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
		DriveTrain->UpdateEBrake(1,150);
	}
#else
	AutonWait2(2.0f,150);
#endif
	autoindex = 3;
	DriveTrain->UpdateEBrake(0,0);
	
	Auto_GYROSTRAIGHT(-.4f,-20,0.0f);
	Auto_GYROSTRAIGHT(-3.5f,-50,-30.0f);

	ShooterWheel->SetState(ShooterState_kinect);
	ShooterWheel->ShooterToggle = -1;
	Auto_GYROSTRAIGHT(-1.0f,-750,-36.0f);//	Auto_GYROSTRAIGHT(-1.0f,-850,-30.0f);
	autoindex = 4;
	AutonWait2(.1f,-10);
	Turret->SetTarg(50.0f);//Turret->SetTarg(37.0f);
	AutonWait(1.0f);
	autoindex = 5;
	AUTO_Wait_Kinect(4);
	autoindex = 6;
	Auto_Kinect_Aim2(0.4f);
	AutonWait(.2f);
	//autoindex = 7;
	//Turret->SetTarg(.5f);
	//AutonWait(.f);
	Auto_FullLaunch();
	FireAll();
}
*/
