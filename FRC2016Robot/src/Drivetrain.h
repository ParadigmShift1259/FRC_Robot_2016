// drivetrain.h


#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_


#include "WPILib.h"
#include "OperatorInputs.h"
#include <driverstation.h>
#include <SpeedController.h>
#include <timer.h>
#include <ADXRS450_Gyro.h>


class Drivetrain
{
public:
	Drivetrain(OperatorInputs *inputs, DriverStation *ds);
	~Drivetrain();
	void Init();
	void Loop();
	void Drive(double x, double y, bool ramp = false);
	void DriveXY(double x, double y, bool ramp = false);
	void Shift();
	// change drivetrain direction and return true if going forward
	bool ChangeDirection();

	double Ramp(double previousPow, double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void resetEncoders();
	double LeftMotor(double &invMaxValueXPlusY);
	double RightMotor(double &invMaxValueXPlusY);
	void SetRatioLR();
	void CheckEncoderTimer();

	bool GetIsDoneDriving() { return m_isdonedriving; }
	bool GetIsTurning() { return m_isturning; }
	void turnAngle();
	void setSpeedPositive();
	void Stop();
	void SetAngle(double angle);
	void DriveDistance(double distance);

	void setCoasting(double newCoasting) {m_coasting = newCoasting;}
	double getLeftPow() {return m_leftpow;}
	double getRightPow() {return m_rightpow;}
	double getRatio() {return m_ratiolr;}
	bool getIsHighGear() {return m_ishighgear;}
	bool getIsLeftFaster() {return m_isleftfaster;}
	double getLeftSpeed() {return m_leftspeed;}
	double getRightSpeed() {return m_rightspeed;}
	//double getRightEncoderPulses() {return rightEncoder->GetRaw();}
	//double getLeftEncoderPulses() {return leftEncoder->GetRaw();}
	//double getRightEncoderDistance() {return rightEncoder->GetDistance();}
	//double getLeftEncoderDistance() {return leftEncoder->GetDistance();}

protected:
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
	bool m_isturning;
	double m_angle;
	bool m_isdonedriving;
	bool m_isdownshifting;

	double m_invertleft;
	double m_invertright;
	double m_direction;

	OperatorInputs *m_inputs;
	DriverStation *m_driverstation;
	CANTalon *m_lefttalonlead;
	CANTalon *m_lefttalonfollow;
	CANTalon *m_righttalonlead;
	CANTalon *m_righttalonfollow;
	Solenoid *m_shifter;
	//Encoder *leftEncoder;
	//Encoder *rightEncoder;

	//ADXRS450_Gyro *gyro;
	double m_prevgyro;
	Timer *m_timerencoder;
	Timer *m_timerautonomous;
};



#endif /* SRC_DRIVETRAIN_H_ */
