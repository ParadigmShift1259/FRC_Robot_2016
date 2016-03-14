// drivetrain.h


#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

#include "WPILib.h"
#include "OperatorInputs.h"
#include <driverstation.h>
#include <SpeedController.h>
#include <timer.h>
#include <Commands/PIDSubsystem.h>
#include "AnglePID.h"
#include "DrivePID.h"


class AnglePID;
class DrivePID;

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
	void Shift(bool);
	// change drivetrain direction and return true if going forward
	bool ChangeDirection();
	void LowSpeedDriving();

	void SetRamp(double);

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
	CANTalon *LeftTalon() {return m_lefttalonlead;}
	CANTalon *RightTalon() {return m_righttalonlead;}
	void EnablePID(double kP, double kI, double kD, double kF, double kPosLeft, double kPosRight);
	void DisablePID(float maxvolt = +12.0f, float minvolt = -12.0f);
	double GetLeftRotations();
	double GetRightRotations();
	void SetDesiredDriveMagnitude(double);
	void SetDesiredDriveAngle(double);
	void DriveAuto();
	double GetAngle();
	double GetDistance();
	void SetDistance(double);
	void SetAngle(double);
	bool AngleReached();
	bool DistanceReached();
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
	AnglePID *m_anglepid;
	DrivePID *m_drivepid;

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

	double m_desiredangle;
	double m_desiredmagnitude;
};

#endif /* SRC_DRIVETRAIN_H_ */

