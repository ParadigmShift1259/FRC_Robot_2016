// drivetrain.h


#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_


#include "WPILib.h"
#include "OperatorInputs.h"
#include <driverstation.h>
#include <CANTalon.h>
#include <timer.h>
#include "Const.h"


#define FIRSTDRIVE


class Drivetrain
{
public:
	Drivetrain(OperatorInputs *inputs, DriverStation *ds);
	~Drivetrain();
#ifdef FIRSTDRIVE
	void TeleopLoop();
	void AutonoumousLoop();
	void Drive(double , double rotation);
	void Shift();
	void OverwrittenShift();
	void UpdateStates();
	void DisableUserControl();
	void EnableUserControl();
	void Stop();
	bool ChangeDirection();
	void ResetEncoderPosition();
	void DisableShift();

	const double GetRightRotations() {return m_righttalonlead->GetPosition()/DRIVE_ENC_CPR;}
	const double GetLeftRotations() {return m_lefttalonlead->GetPosition()/DRIVE_ENC_CPR;}
#else
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

	double Ramp(double previousPow, double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax);
	double LeftMotor(double &invMaxValueXPlusY);
	double RightMotor(double &invMaxValueXPlusY);
	void SetRatioLR();
	void ResetEncoders();
	void CheckEncoderTimer();

	void setCoasting(double newCoasting) {m_coasting = newCoasting;}
	double getLeftPow() {return m_leftpow;}
	double getRightPow() {return m_rightpow;}
	double getRatio() {return m_ratiolr;}
	bool getIsHighGear() {return m_ishighgear;}
	bool getIsLeftFaster() {return m_isleftfaster;}
	double getLeftSpeed() {return m_leftspeed;}
	double getRightSpeed() {return m_rightspeed;}
	//double getRightEncoderPulses() {return m_rightencoder->GetRaw();}
	//double getLeftEncoderPulses() {return m_leftencoder->GetRaw();}
	//double getRightEncoderDistance() {return m_rightencoder->GetDistance();}
	//double getLeftEncoderDistance() {return m_leftencoder->GetDistance();}
#endif

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
#if defined FIRSTDRIVE
	bool m_shiftstate;
	bool m_requestshift = false;
	bool m_shiftbraking = false;
	bool m_usercontrol= true;
	bool m_userforward;
	RobotDrive *m_robotdrive;
#else
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

	double m_invertleft;
	double m_invertright;
	double m_direction;
#endif
};



#endif /* SRC_DRIVETRAIN_H_ */
