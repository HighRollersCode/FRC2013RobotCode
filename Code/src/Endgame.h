#ifndef ENDGAME_H
#define ENDGAME_H
#include "WPIlib.h"
#include "Ace_Defines.h"
class EndgameClass
{
private:

	int CurrentDeploy;
	int PreviousDeploy;
	int Curr_State;
	Solenoid *HooksDeploy;
#if PRACTICE_BOT
	Solenoid *HooksRetract;
#endif
public:
	int webehangin;
	EndgameClass();
	~EndgameClass();
	void SendData();
	void UpdateEndgameOperator(int ToggleBtn);
};
#endif

