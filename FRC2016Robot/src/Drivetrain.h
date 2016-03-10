// drivetrain.h


#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_


#include "WPILib.h"
#include "OperatorInputs.h"
#include <driverstation.h>
#include <SpeedController.h>
#include <timer.h>


class Drivetrain
{
public:
	Drivetrain(OperatorInputs *inputs, DriverStation *ds);
	~Drivetrain();
	void Init();
	void Loop();
	void Stop();
	void Drive(double x, double y, bool ramp = false);
	//void DriveXY(double x, double y, bool ramp = false);
	//void setPowerXYleft(double joyStickX, double joyStickY);
	//void setPowerXYright(double joyStickX, double joyStickY);
	void Shift();
	// change drivetrain direction and return true if going forward
	bool ChangeDirection();
	void LowSpeedDriving();

	double LeftMotor(double &invMaxValueXPlusY);
	double RightMotor(double &invMaxValueXPlusY);
	void SetRatioLR();
	void ResetEncoders();
	void CheckEncoderTimer();

	double Ramp(double previousPow, double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax);
	void setCoasting(double newCoasting) {m_coasting = newCoasting;}
	double getLeftPow() {return m_leftpow;}
	double getRightPow() {return m_rightpow;}
	double getRatio() {return m_ratiolr;}
	bool getIsHighGear() {return m_ishighgear;}
	bool getIsLeftFaster() {return m_isleftfaster;}
	double getLeftSpeed() {return m_leftspeed;}
	double getRightSpeed() {return m_rightspeed;}
	double getLeftPosition() {return m_lefttalonlead->GetPosition();}
	double getRightPosition() {return m_righttalonlead->GetPosition();}
	void resetLeftEncoder() {m_lefttalonlead->SetEncPosition(0);}
	void resetRightEncoder() {m_righttalonlead->SetEncPosition(0);}
	//double getRightEncoderPulses() {return m_rightencoder->GetRaw();}
	//double getLeftEncoderPulses() {return m_leftencoder->GetRaw();}
	//double getRightEncoderDistance() {return m_rightencoder->GetDistance();}
	//double getLeftEncoderDistance() {return m_leftencoder->GetDistance();}

protected:
	OperatorInputs *m_inputs;
	DriverStation *m_driverstation;
	CANTalon *m_lefttalonlead;
	CANTalon *m_lefttalonfollow;
	CANTalon *m_righttalonlead;
	CANTalon *m_righttalonfollow;
	Solenoid *m_shifter;
	//Encoder *m_leftencoder;
	//Encoder *m_rightencoder;
	Timer *m_timerencoder;

	double m_leftpow;
	double m_rightpow;
	double m_leftencodermax;
	double m_rightencodermax;
	double m_ratiolr;
	double m_leftencoderfix;
	double m_rightencoderfix;
	bool m_ishighgear; //Robot starts in low gear
	bool m_isleftfaster;
	double m_leftspeed;
	double m_rightspeed;
	double m_previousx;
	double m_previousy;
	double m_leftposition;
	double m_rightposition;
	double m_coasting;
	bool m_isdownshifting;
	bool m_lowspeedmode;
	bool m_shift;

	double m_invertleft;
	double m_invertright;
	double m_direction;
};



#endif /* SRC_DRIVETRAIN_H_ */
