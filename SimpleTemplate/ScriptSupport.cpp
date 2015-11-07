#include "MyRobot.h"
#include "HrScript.h"
#include "HRLogger.h";

// Create commands for the robot scripts

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Settings Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetTurretPConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "TurretSettings"; }		
	virtual int Get_Parameter_Count() { return 5; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTurretPConstantsCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->Turret->TurretL_Limit = (int)m_Parameters[0];
		RobotDemo::Get()->Turret->TurretR_Limit = (int)m_Parameters[1];
		RobotDemo::Get()->Turret->TurretYawK = (float)m_Parameters[2];
		RobotDemo::Get()->Turret->TurretYawMin = (float)m_Parameters[3];
		RobotDemo::Get()->Turret->MaximumXError = (float)m_Parameters[4];
		//TiltMin = m_Parameters[1];
	}
};
class SetTiltPConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "TiltSettings"; }		
	virtual int Get_Parameter_Count() { return 4; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTiltPConstantsCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->Turret->TiltMultiplier = (float)m_Parameters[0];
		RobotDemo::Get()->Turret->TiltMin = (float)m_Parameters[1];
		RobotDemo::Get()->Turret->PitchScale = (float)m_Parameters[2];
		RobotDemo::Get()->Turret->MaximumYError = (float)m_Parameters[3];
		//TiltMin = m_Parameters[1];
	}
};
class SetShooterPConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "ShooterSettings"; }		
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new SetShooterPConstantsCommand(); }
	virtual void Execute()
	{
		ShooterWheelClass::Shooter_WheelK = m_Parameters[0];
		RobotDemo::Get()->Launcher->rapiddelay = (float)m_Parameters[1];
	}
};


class SetEBrakeConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "EBrakeSettings"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetEBrakeConstantsCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->DriveTrain->Ebrakemult = (float)m_Parameters[0];

		//TiltMin = m_Parameters[1];
	}
};

class SetDriveTrainConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DrainTrainSettings"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetDriveTrainConstantsCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->turningp = (float)m_Parameters[0];

		//TiltMin = m_Parameters[1];
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Wait Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



class WaitCommand1 : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Wait"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitCommand1(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->AutonWait(m_Parameters[0]);
	}
};
class WaitForBrakeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "BrakeWait"; }		
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitForBrakeCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->AutonWait2(m_Parameters[0],(int)m_Parameters[1]);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Driving Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
								
class DisableBrakeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DisableBrake"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new DisableBrakeCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->AutonWait2(m_Parameters[0],(int)m_Parameters[1]);
	}
};
class DriveHeadingTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicksHeading"; }		
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveHeadingTicksCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Auto_GYROSTRAIGHT(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
	}
};
class DriveTurnTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTurnTicks"; }		
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTurnTicksCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Auto_DriveEncoder(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
	}
};
class GyroTurnCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "GyroTurn"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new GyroTurnCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Auto_GYROTURN(m_Parameters[0]);
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Intake Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class IntakeOnCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "IntakeOn"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new IntakeOnCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Intake->IntakeOn();
	}
};
class IntakeOffCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "IntakeOff"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new IntakeOffCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Intake->IntakeOff();
	}
};
class RampUpCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "RampUp"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new RampUpCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->Intake->RampUp->Set(false);
		RobotDemo::Get()->Intake->RampDown->Set(true);
	}
};
class RampDownCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "RampDown"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new RampDownCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->Intake->RampUp->Set(true);
		RobotDemo::Get()->Intake->RampDown->Set(false);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Turret, Tilt Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class SetTurretCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetTurret"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTurretCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Turret->SetTargABS(0);
		RobotDemo::Get()->AutonomousContol->Turret->targetTurretEnc = (int)(m_Parameters[0]);
		
	}
};
class SetTurretGyroCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetTurretGyro"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTurretGyroCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Turret->SetTargABS(m_Parameters[0]);
	}
};
class SetTiltCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetTilt"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTiltCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Turret->TargetTiltEnc = (int)(m_Parameters[0]);
	}
};
class CameraAimCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "CameraAim"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new CameraAimCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Auto_CameraAim();
	}
};
class ResetShooterCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "ResetShooter"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new ResetShooterCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->Turret->UpdateReset(1,1,1);
	}
};
class SetPopperCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetPopper"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetPopperCommand(); }
	virtual void Execute()
	{
		if(m_Parameters[0] == 1)
			RobotDemo::Get()->Popper->Actuator->Set(true);
		else
			RobotDemo::Get()->Popper->Actuator->Set(false);
		
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Shooter Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



class ShooterStartCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "ShooterStart"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new ShooterStartCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->ShooterWheel->SetState(ShooterState_overrideRPM);
		//RobotDemo::Get()->ShooterWheel->ShooterOverrideRPM(4700.0f);
		RobotDemo::Get()->ShooterWheel->ShooterOverrideRPM(5500.0f);
		RobotDemo::Get()->ShooterWheel->ShooterToggle = -1;
	}
};
class ShooterEndCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "ShooterEnd"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new ShooterStartCommand(); }
	virtual void Execute()
	{
		//RobotDemo::Get()->ShooterWheel->SetState(ShooterState_overrideRPM);
		//RobotDemo::Get()->ShooterWheel->ShooterOverrideRPM(4700.0f);
		RobotDemo::Get()->ShooterWheel->ShooterToggle = 1;
	}
};


class LaunchCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Launch"; }		
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new LaunchCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->AutonomousContol->Auto_FullLaunch();
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Support Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class SetAutoCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetAuto"; }		
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetAutoCommand(); }
	virtual void Execute()
	{
		RobotDemo::Get()->Auto_Index = (int)m_Parameters[0];
	}
};

void RobotDemo::Init_Script_System()
{
	m_ScriptSystem = new HrScriptSystemClass();
	printf("Script Initializing \r\n");
	// register commands with the system	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	m_ScriptSystem->Add_Command(new SetShooterPConstantsCommand());
	m_ScriptSystem->Add_Command(new SetTurretPConstantsCommand());
	m_ScriptSystem->Add_Command(new SetTiltPConstantsCommand());
	m_ScriptSystem->Add_Command(new SetEBrakeConstantsCommand());
	m_ScriptSystem->Add_Command(new SetDriveTrainConstantsCommand());
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	m_ScriptSystem->Add_Command(new WaitCommand1());
	m_ScriptSystem->Add_Command(new WaitForBrakeCommand());
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	m_ScriptSystem->Add_Command(new DisableBrakeCommand());
	m_ScriptSystem->Add_Command(new DriveHeadingTicksCommand());
	m_ScriptSystem->Add_Command(new DriveTurnTicksCommand());
	m_ScriptSystem->Add_Command(new GyroTurnCommand());
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	m_ScriptSystem->Add_Command(new IntakeOnCommand());
	m_ScriptSystem->Add_Command(new IntakeOffCommand());
	m_ScriptSystem->Add_Command(new RampUpCommand());
	m_ScriptSystem->Add_Command(new RampDownCommand());
	
	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetTurretCommand());
	m_ScriptSystem->Add_Command(new SetTurretGyroCommand());
	m_ScriptSystem->Add_Command(new SetTiltCommand());
	m_ScriptSystem->Add_Command(new CameraAimCommand());	
	m_ScriptSystem->Add_Command(new ResetShooterCommand());
	m_ScriptSystem->Add_Command(new SetPopperCommand());
	
	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new ShooterStartCommand());
	m_ScriptSystem->Add_Command(new ShooterEndCommand());
	m_ScriptSystem->Add_Command(new LaunchCommand());
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	m_ScriptSystem->Add_Command(new SetAutoCommand());
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	Load_Scripts();
	
}
void RobotDemo::Load_Scripts()
{
	// Load and run the robot settings script:
	//m_ScriptSystem->Load_And_Run_Script("RobotSettings.hrs");

	// Load all of the auto-mode scripts
	
	
	//7-disc Autos//
	m_ScriptSystem->Set_Auto_Script(1,"AUTO_FUNNEL.hrs");
	
	//Centerline Autos//
	m_ScriptSystem->Set_Auto_Script(2,"AUTO_CENTERLINE.hrs");
	m_ScriptSystem->Set_Auto_Script(3,"AUTO_CENTERLINE_ONESHOT.hrs");
	m_ScriptSystem->Set_Auto_Script(4,"AUTO_CENTERLINE_SWOOP.hrs");
	
	//FrontLine Autos//
	m_ScriptSystem->Set_Auto_Script(5,"AUTO_FRONTLINE.hrs");
	
	//6 Disc Counter Auto
	m_ScriptSystem->Set_Auto_Script(6,"AUTO_COUNTER.hrs");
	
	//Basic Last-Resort Autos
	m_ScriptSystem->Set_Auto_Script(7,"AUTO_BASIC_3SHOT.hrs");
	m_ScriptSystem->Set_Auto_Script(8,"AUTO_CHEYENNE.hrs");
	
	//Settings
	m_ScriptSystem->Set_Auto_Script(0,"RobotSettings.hrs");
}
