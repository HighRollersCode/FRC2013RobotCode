#ifndef AUTON_H
#define AUTON_H

#include "WPILib.h"
#include "DriveTrain.h"
#include "Intake.h"
#include "Launcher.h"
#include "ShooterWheel.h"
#include "Turret.h"
#include "Watchdog.h"


class AutonomousController
{
	
private:
	Watchdog *watchdog;
public:
	DriveTrainClass *DriveTrain;
	IntakeClass *Intake;
	LauncherClass *Launcher;
	ShooterWheelClass *ShooterWheel;
	TurretClass *Turret;
	DriverStation *ds;
	Timer *AutonTimer;
	Timer *SendTimer;
	float turningp;
	bool Abort;


	bool Running();
	void AutonWait(float Seconds);
	void AutonWait2(float Seconds, int brake);
	bool Auto_System_Update();
	void Auto_FullLaunch()
	{
		if(Running())Launcher->FullLaunch();
	}
	void Auto_IntakeOn()
	{
		if(Running())Intake->IntakeOn();
	}
	void Auto_IntakeOff()
	{
		if(Running())Intake->IntakeOff();
	}
	
	
	void Auto_DriveTimer(float Forward, float Turn, float Seconds);

	void Auto_GYROTURN(float heading);
	void Auto_GYROSTRAIGHT(float forward,float ticks, float desheading);
	void Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks);
	//void Au
	void SendData();

	
	void Auto_DriveEncoder(float Forward, float Turn, float Ticks);
	void Auto_CameraAim();
	AutonomousController
	(
		DriveTrainClass *D,
		IntakeClass *I,
		LauncherClass *L,
		ShooterWheelClass *S,
		TurretClass *T,
		Watchdog *W,
		DriverStation *Ds
	);
	~AutonomousController(){}
	void Auto_Start();
	void Auto_End();
	void AUTO_eating();
	void AUTO_Bridge_Score();
	void AUTO_Layup();
	void AUTO_2ball();
	void AUTO_BridgeSteal();
	void AUTO_AllianceBridge();
	void FireAll();

};
#endif

