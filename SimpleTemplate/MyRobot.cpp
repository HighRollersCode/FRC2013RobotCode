

#include "MyRobot.h"
#include "HRLogger.h"
RobotDemo * RobotDemo::TheRobot = NULL;
RobotDemo::RobotDemo(void)
{
	HRLogger::Init(false);
	p_8 = new DigitalOutput(11); //Relay(8,Relay::kForwardOnly);
	p_7 = new DigitalOutput(12);// Relay(7,Relay::kForwardOnly);
	p_6 = new DigitalOutput(13);//Relay(6,Relay::kForwardOnly);

	PORT_8 = 0;
	PORT_7 = 0;
	PORT_6 = 0;
	
	Auto_Index = 1;
	TheRobot = this;
	SendDataCurr = 0;
	SendDataPrev = 0;
	SendDataToggle = 1;
	leftStick = new Joystick(1);
	rightStick = new Joystick(2);			// create the joysticks
	turretStick = new Joystick(3);
	HRLogger::Log("joysticksgood\n");
	ds = DriverStation::GetInstance();
	DriveTrain = new DriveTrainClass();
	HRLogger::Log("drivetraingood\n");
	Intake = new IntakeClass();
	HRLogger::Log("intakegood\n");
	Launcher = new LauncherClass();
	HRLogger::Log("launchergood\n");
	ShooterWheel = new ShooterWheelClass();
	HRLogger::Log("wheelgood\n");
	Turret = new TurretClass();
	HRLogger::Log("turretgood\n");
#if !PRACTICE_BOT
	Popper = new PopperClass();
#endif
	Endgame = new EndgameClass();
	Turret->Reset_Sensors();
	
	AutonomousContol = new AutonomousController
	(
		DriveTrain,
		Intake,
		Launcher,
		ShooterWheel,
		Turret,
		&GetWatchdog(),
		ds
	);
	compressor = new Compressor(14,1);	//Pressure Switch, Compressor Relay 
	compressor->Start();
	GameTimer = new Timer();
	
	SendTimer = new Timer();
	SendTimer->Reset();
	SendTimer->Start();
	Init_Script_System();
	HRLogger::Log("endofinit\n");
	ARDUINO_SET(1,0,0);


	ARDUINO_SEND();
	printf("gogogo\r\n");

}
RobotDemo::~RobotDemo(void)
{
	TheRobot = NULL;
}
void RobotDemo::UpdateDash()
{
	if ( NetworkTable::GetTable("987Table") != NULL)
	{
		try
		{
			SmartDashboard::init();
			if(SendTimer->Get() > .05)
			{
				if(SendDataToggle == -1)
				{
					SendTimer->Reset();
					SendTimer->Start();
				//	printf("resetdash\n");
					static double beat = 0;
					beat += 0.01f;
				//	printf("heartbeatupdate\n");
					SmartDashboard::PutNumber("Heartbeat",beat);
				//	printf("sendheartbeat\n");
					DriveTrain->SendData();
				//	printf("dtsend\n");
					Turret->SendData();
				//	printf("turrsend\n");
				//	Intake->SendData();
					ShooterWheel->SendData();
				//	printf("shootersend\n");
					SmartDashboard::PutNumber("Override",ShooterWheel->OverrideCommand);
					//printf("sendoverride\n");
				}
				SmartDashboard::PutNumber("^",ShooterWheel->INDICATOR);
				SmartDashboard::PutBoolean("OnTarget",Turret->Ontarget);
				SmartDashboard::PutBoolean("Compressor",compressor->GetPressureSwitchValue());
			}
			Turret->GetData();
		}
		catch(exception)
		{
					
		}
	}
	//printf("afterturrget\n");
}
void RobotDemo::ARDUINO_SEND()
{
	p_8->Set(PORT_8);
	p_7->Set(PORT_7);
	p_6->Set(PORT_6);
	/*
	if(PORT_8 == 1)p_8->Set(Relay::kForward);
	else p_8->Set(Relay::kOff);
	
	if(PORT_7 == 1)p_7->Set(Relay::kForward);
	else p_7->Set(Relay::kOff);
	
	if(PORT_6 == 1)p_6->Set(Relay::kForward);
	else p_6->Set(Relay::kOff);
	*/
	
}

void RobotDemo::ARDUINO_SET(int p6, int p7, int p8)
{
	PORT_6 = p6;
	PORT_7 = p7;
	PORT_8 = p8;
}
void RobotDemo::Autonomous(void)
{	
	//GetWatchdog().SetEnabled(true);
	HRLogger::Log("FMS ATTATCHED %d\n",DriverStation::GetInstance()->IsFMSAttached());
	HRLogger::Log("AUTO BEGIN \n");
	Load_Scripts();
	printf("loaded\n");
	m_ScriptSystem->Run_Auto_Script(0);
	printf("ransettings\n");
	AutonomousContol->Auto_Start();
	printf("startfunction\n");
	//Load_Scripts();
	//printf("Loaded\r\n");
	

	m_ScriptSystem->Run_Auto_Script(Auto_Index);
	HRLogger::Log("runauto\n");
	AutonomousContol->Auto_End();
	HRLogger::Log("autoend\n");
}
void RobotDemo::OperatorControl(void)
{
	printf("1\r\n");
	HRLogger::Log("FMS ATTATCHED %d\n",DriverStation::GetInstance()->IsFMSAttached());
	HRLogger::Log("TELEOP BEGIN \n");	
	GetWatchdog().SetEnabled(true);
	HRLogger::Log("changedwatch\n");
	GetWatchdog().SetExpiration(.1);
	GetWatchdog().Feed();	
	ShooterWheel->WheelOff();
	GetWatchdog().Feed();	
	DriveTrain->ResetEncoders_Timers();
	GetWatchdog().Feed();	
	Launcher->ResetTimers();
	GameTimer->Reset();
		GameTimer->Start();
HRLogger::Log("reseteveryting\n"
			);
	
	
	Load_Scripts();
	m_ScriptSystem->Run_Auto_Script(0);


	Turret->Reset_Sensors();
	DriveTrain->ResetEncoders_Timers();
	printf("2\r\n");

	while (IsOperatorControl())
	{
		//printf("insideloop\n");
		//printf("3\r\n");
		GetWatchdog().Feed();	
		DriveTrain->Drive_Operator(-leftStick->GetY(),-rightStick->GetY(),rightStick->GetRawButton(11));
		//printf("driveloop\n");
		DriveTrain->Shifter_Update(rightStick->GetTrigger());
		//printf("shifterloop\n");
		DriveTrain->UpdateEBrake(rightStick->GetRawButton(11),0);
		//printf("ebrakeloop\n");
		Intake->UpdateRampInput(rightStick->GetRawButton(2));
		Endgame->UpdateEndgameOperator(leftStick->GetRawButton(6));
		Intake->UpdateIntake(leftStick->GetRawButton(2),leftStick->GetTrigger(),rightStick->GetRawButton(2));
		//printf("intakeloop\n");
		Turret->UpdateTurret(turretStick->GetX()*.6f,turretStick->GetRawButton(2),turretStick->GetRawButton(7));
		//printf("4\r\n");
		//printf("turretloop\n");
		Turret->UpdateTilt(turretStick->GetY(),turretStick->GetRawButton(3),turretStick->GetRawButton(2),turretStick->GetRawButton(7));
		//printf("tiltloop\n");
		Turret->UpdateReset(turretStick->GetRawButton(6),1,1);
		Turret->UpdateReset(turretStick->GetRawButton(11),1,-60);
		//printf("turresetloop\n");
		Launcher->UpdateLaunch_Operator(turretStick->GetTrigger(),turretStick->GetRawButton(9));
		//printf("launchloop\n");
		//ShooterWheel->ShooterOverride(turretStick->GetThrottle());
		/*
		ShooterWheel->UpdateShooter(turretStick->GetRawButton(4),
			turretStick->GetRawButton(11),
				turretStick->GetRawButton(11),
				turretStick->GetRawButton(10),
				GameTimer->Get());
				*/
		ShooterWheel->UpdateShooter(turretStick->GetRawButton(4),
		turretStick->GetRawButton(10),GameTimer->Get());
		//printf("shooterloop\n");
		ShooterWheel->OverrideCommand = (turretStick->GetThrottle()+.5f)*5000.0f;
		//ShooterWheel->OverrideCommand = turretStick->GetY();
		
		//printf("shooterloop2\n");
#if !PRACTICE_BOT
		Popper->UpdatePopper(turretStick->GetRawButton(5));
#endif
		
		//	printf("endgameloop\n");
		//printf("6\r\n");
		UpdateDash();
	//	printf("7\r\n");
		//	printf("updatedashloop\n");
		//	float g = (float)((leftStick->GetThrottle()+1)/2);
		SendDataPrev = SendDataCurr;
		SendDataCurr = turretStick->GetRawButton(8);
		if((SendDataCurr == 1)&&(SendDataPrev == 0))
		{
			SendDataToggle = -SendDataToggle;
		}
		if(leftStick->GetRawButton(8) == 1)
		{
			Load_Scripts();
			m_ScriptSystem->Run_Auto_Script(0);
		}
		if(leftStick->GetRawButton(9) == 1)
		{
			Turret->TargetTiltEnc = 0;
			Turret->SetTargABS(0);
			Turret->Reset_Sensors();
			DriveTrain->ResetEncoders_Timers();
			
		}
		GetWatchdog().Feed();	
		//printf("fed\n");
		Wait(.001f);
		//printf("pastwait\n");
		ARDUINO_SEND();
		GetWatchdog().Feed();	
	}
}
START_ROBOT_CLASS(RobotDemo);
