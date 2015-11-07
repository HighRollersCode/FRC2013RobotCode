
#include "Popper.h"
#include "Ace_Defines.h"

PopperClass::PopperClass()
{
	ResetTimer = new Timer();
#if !PRACTICE_BOT
	Actuator = new Solenoid(1,8);
#endif
	
	CurrPop = 0;
	PrevPop = 0;
}

void PopperClass::UpdatePopper(int enable)
{
	Actuator->Set(enable==1);
	/*
	PrevPop = CurrPop;
	CurrPop = enable;
	if((PrevPop == false) && (CurrPop == true))
	{
		Actuate();
	}
	if(ResetTimer->Get() >= .125)
	{
		Actuator->Set(false);
	}
	*/
}
void PopperClass::Actuate()
	{
		ResetTimer->Reset();
		ResetTimer->Start();
#if !PRACTICE_BOT
		Actuator->Set(true);
#endif
	}
