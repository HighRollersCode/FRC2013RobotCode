
#include "Endgame.h"
#include "MyRobot.h"

EndgameClass::EndgameClass()
{
	CurrentDeploy = 0;
	PreviousDeploy = 0;
	Curr_State = -1;
	webehangin = -1;
	HooksDeploy = new Solenoid(7);
#if PRACTICE_BOT
	HooksRetract = new Solenoid(8);
#endif
}
void EndgameClass::UpdateEndgameOperator(int ToggleBtn)
{
	PreviousDeploy = CurrentDeploy;
	CurrentDeploy = ToggleBtn;
	if((PreviousDeploy == 0)&&(CurrentDeploy == 1))
	{
		Curr_State *=-1;webehangin *= -1;
	}
	if(Curr_State == -1)
	{
		HooksDeploy->Set(false);
#if PRACTICE_BOT
				HooksRetract->Set(true);
#endif
	}
	else if (Curr_State == 1)
	{
		HooksDeploy->Set(true);
#if PRACTICE_BOT
		HooksRetract->Set(false);
#endif
		RobotDemo::Get()->Turret->targetTurretEnc = 1;
		RobotDemo::Get()->Turret->TargetTiltEnc =1;
		
		//RobotDemo::Get()->Intake->RampStatus = 1;
		RobotDemo::Get()->ARDUINO_SET(1,0,0);
		

	}
	
	
	
	
	
}
