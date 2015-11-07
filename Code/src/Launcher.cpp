#include "Launcher.h"
#include "MyRobot.h"
static float launchdelay = .125f;

LauncherClass::LauncherClass()
{
	LaunchTrig = false;
	prevLaunchTrig = false;
	currentTestBTN = 0;
	prevTestBTN = 0;
	LaunchOut = new Solenoid(1,3);
	LaunchIn = new Solenoid(1,4);
	rapiddelay = .4f;
	Launch_DelayTM = new Timer();
	RapidFireTimer = new Timer();
	LaunchState = "";
}
void LauncherClass::Launch()
{
	RobotDemo::Get()->ARDUINO_SET(1,0,1);
	Launch_DelayTM->Reset();
	Launch_DelayTM->Start();
	RapidFireTimer->Reset();
	RapidFireTimer->Start();
	LaunchOut->Set(true);
	LaunchIn->Set(false);
}
void LauncherClass::FullLaunch()
{
	Launch();
	while(Launch_DelayTM->Get() < launchdelay)
	{
		RobotDemo::Get()->GetWatchdog().Feed();
	}
	LaunchOut->Set(true);	
	LaunchIn->Set(false);
}
void LauncherClass::UpdateLaunch_Operator(bool EnableTrig,int fullcourtbtn)//turretstickgettrig, turretstickgetbtn6
{
	prevLaunchTrig = LaunchTrig;
	LaunchTrig = EnableTrig;

	if((LaunchTrig == 1)&& (prevLaunchTrig == 0))
	{
		Launch();
		
	}
	int rpmdes = 0;
	
	if(fullcourtbtn == 1)
	{
	//	rpmdes = 4799;
		rpmdes = 5200;
		rapiddelay =.5f;
	}
	else
	{
		//rpmdes = 4600;
        rpmdes = 5000;
		rapiddelay = .4f;
	}
	if(LaunchTrig == 1)
	{
		if(RapidFireTimer->Get() > rapiddelay)
		{
			if(RobotDemo::Get()->ShooterWheel->RPM >= rpmdes)
			{
				if(fullcourtbtn ==1)
				{
					if(RobotDemo::Get()->ShooterWheel->RPM <= 5200+70)
					{
						Launch();
					}
				}
				else
				{
					Launch();
				}
			}
		}
	}
	else
	{
		RapidFireTimer->Stop();
	}
	if(Launch_DelayTM->Get() > launchdelay)
	{
		LaunchOut->Set(false);	
		LaunchIn->Set(true);
	//	RobotDemo::Get()->ARDUINO_SET(0,0,0);
	}
}

void LauncherClass::ResetTimers()
{
	Launch_DelayTM->Stop();
	Launch_DelayTM->Reset();
}
