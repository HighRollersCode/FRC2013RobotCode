#ifndef LAUNCHER_H
#define LAUNCHER_H
#include "WPIlib.h"
#include "Ace_Defines.h"
class LauncherClass
{
	bool LaunchTrig;
	bool prevLaunchTrig;
	
	int currentTestBTN;
	int prevTestBTN;
	
		
	Solenoid *LaunchOut;
	Solenoid *LaunchIn;

	Timer *Launch_DelayTM;
	
	Timer *RapidFireTimer;
	
	string LaunchState;
	
public:
	float rapiddelay;
	bool HoldingBall;
		
	LauncherClass();
	~LauncherClass();
	
	void Launch();
	void FullLaunch();
	void UpdateLaunch_Operator(bool EnableTrig,int fullcourtbtn);
	
	void ResetTimers();
	void SendData();
};
#endif
