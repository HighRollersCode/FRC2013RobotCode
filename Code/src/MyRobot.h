#ifndef MYROBOT_H
#define MYROBOT_H

#include "WPILib.h"
//#include "Talon.h"
#include "DriveTrain.h"
#include "Intake.h"
#include "Launcher.h"
#include "ShooterWheel.h"
#include "Turret.h"
#include "Auton.h"
#include "math.h"
#include "HrScript.h"
#include "Endgame.h"
#include "Popper.h"
#include "Ace_Defines.h"

class RobotDemo: public SimpleRobot
{
public:	
	DigitalOutput *p_8;
	DigitalOutput *p_7;
	DigitalOutput *p_6;
	int PORT_8;
	int PORT_7;
	int PORT_6;
	int SendDataCurr;
	int SendDataPrev;
	int SendDataToggle;
	int Auto_Index;
	Joystick *rightStick;			// joystick 2
	Joystick *leftStick;			// joystick 1
	Joystick *turretStick;			// joystick 3
	Joystick *Gamepad1;
	DriverStation *ds;				// driver station object
	DriveTrainClass *DriveTrain;
	IntakeClass *Intake;
	LauncherClass *Launcher;
	ShooterWheelClass *ShooterWheel;
	TurretClass *Turret;
#if !PRACTICE_BOT
	PopperClass *Popper;
#endif
	EndgameClass *Endgame;
	
	Timer *GameTimer;
	Timer *SendTimer;
	
	Compressor *compressor;
	RobotDemo(void);
	~RobotDemo(void);
	AutonomousController *AutonomousContol;
	static RobotDemo * Get() { return TheRobot; }
	void UpdateDash();
	void Autonomous(void);
	void OperatorControl(void);
	void Init_Script_System();
	void Load_Scripts();
	void ARDUINO_SET(int p6, int p7, int p8);
	void ARDUINO_SEND();
protected:

	static RobotDemo * TheRobot;	
	HrScriptSystemClass * m_ScriptSystem;
};

#endif
