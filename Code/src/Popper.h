#ifndef POPPER_H
#define POPPER_H

#include "WPILib.h"

class PopperClass
{	
	Timer *ResetTimer;

	int CurrPop;
	int PrevPop;
public:
	#if !PRACTICE_BOT
	Solenoid *Actuator;
#endif
	PopperClass();
	~PopperClass();
	void UpdatePopper(int enable);
	void Actuate();

};
#endif
