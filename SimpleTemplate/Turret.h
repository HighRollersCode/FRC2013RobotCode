#ifndef TURRET_H
#define TURRET_H
#include "WPIlib.h"

class TurretClass
{
private:
	float currentTurretStickX;
	float previousTurretStickX;
	float turretcommand;
	
	

	int currentholdgyro;
	int prevholdgyro;
	int ClickIndexNew;
	
	int currentEnableAuto;
	int previousEnableAuto;
	
	double TargetX;
	int ClickNumber;
	double AngleX;
	
	double TargetY;
	double AngleY;
	
	
	Victor *Turret;
	Victor *Tilt;
	
	Encoder *TurretEnc;
	Encoder *TiltEnc;
	Timer *ResetTimer;
public:
	bool Ontarget;
	float MaximumXError;
	float MaximumYError;
	double Auto_AngleY;
	double Auto_AngleX;
	int current_ClickIndex;
		int previous_ClickIndex;
	float currentTurretPos;
	float targetTurretPos;
	int currentTurretEnc;
	int targetTurretEnc;
	Gyro *TurretGyro;
	TurretClass();
	~TurretClass();
	void HoldTurret(int input);
	void SetTarg(float v);
	void SetTargABS(float v);
	void UpdateTurret(float Input, int lock, int enableauto);
	void Reset_Sensors();

	void SendData();
	void GetData();
	void UpdateTilt(float Input, int Down, int Up, int enableauto);
	void UpdateReset(int Input, int turretenc, int tiltenc);  
	
	float GetTiltTicks(float Degrees);
	float GetTiltDegrees(float Ticks); 
	 
	float CurrTiltInput;
	float PrevTiltInput;
	int TargetTiltEnc;
	int CurrentTiltEnc;
	float TiltMultiplier;
	int TurretL_Limit;
	int TurretR_Limit;
	float TiltMin;
	float TurretYawK;
	float TurretYawMin;
	float PitchScale;
	int CurrUp;
	int PrevUp;
	int CurrDown;
	int PrevDown;
	int CurrReset;
	int PrevReset;
	int maxtilt;
};
#endif

