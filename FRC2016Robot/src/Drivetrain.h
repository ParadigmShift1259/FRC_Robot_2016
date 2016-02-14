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
	void setPower();
	void setPowerXY(double joyStickX, double joyStickY);
	void childProofShift();
	// change drivetrain direction and return true if going forward
	bool ChangeDirection();

	double rampInput(double previousPow, double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void resetEncoders();
	double fix(double v, double &invMaxValueXPlusY);
	double LeftMotor(double &invMaxValueXPlusY);
	double RightMotor(double &invMaxValueXPlusY);
	void compareEncoders();
	void breakTime();

	bool getIsDoneDriving();
	bool getIsTurning();
	void turnAngle();
	void setSpeedPositive();
	void setGearLow();
	void setAngle(double angle);
	void driveDistance(double distance);

	void setCoasting(double newCoasting) {coasting = newCoasting;}
	double getLeftPow() {return leftPow;}
	double getRightPow() {return rightPow;}
	double getRatio() {return ratio;}
	bool getIsHighGear() {return isHighGear;}
	bool getIsLeftFaster() {return isLeftFaster;}
	double getLeftSpeed() {return leftSpeed;}
	double getRightSpeed() {return rightSpeed;}
	//double getRightEncoderPulses() {return rightEncoder->GetRaw();}
	//double getLeftEncoderPulses() {return leftEncoder->GetRaw();}
	//double getRightEncoderDistance() {return rightEncoder->GetDistance();}
	//double getLeftEncoderDistance() {return leftEncoder->GetDistance();}

private:
	void shift(); //moved to protected to prevent people from accidentally calling it in Robot.cpp

protected:
	double leftPow;
	double rightPow;
	double maxLeftEncoderRate;
	double maxRightEncoderRate;
	double ratio;
	double rightEncoderFix;
	double leftEncoderFix;
	bool isHighGear; //Robot starts in low gear
	bool isLeftFaster;
	double leftSpeed;
	double rightSpeed;
	double previousX;
	double previousY;
	double leftPosition;
	double rightPosition;
	double coasting;
	bool isTurning;
	double angle;
	bool isDoneDriving;
	bool isDownShifting;

	double invertLeft;
	double invertRight;
	double direction;

	OperatorInputs *operatorInputs;
	DriverStation *driverstation;
	CANTalon *leftTalons;
	CANTalon *leftTalons1;
	CANTalon *rightTalons;
	CANTalon *rightTalons1;
	Solenoid *gearShift;
	//Encoder *leftEncoder;
	//Encoder *rightEncoder;

	//ADXRS450_Gyro *gyro;
	double prevGyro;
	Timer *timer;
	Timer *timer1;
};



#endif /* SRC_DRIVETRAIN_H_ */
