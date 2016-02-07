// drivetrain.h


#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_


#include "WPILib.h"
#include <SpeedController.h>
#include "OperatorInputs.h"
#include <driverstation.h>
#include <timer.h>
#include <ADXRS450_Gyro.h>
class Drivetrain
{
public:

	Drivetrain(OperatorInputs *inputs, DriverStation *ds);
	~Drivetrain();
	void Init();
	void driveDistance(double distance);
	double rampInput(double previousPow, double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax);
	//void resetEncoders();
	double fix(double v, double &invMaxValueXPlusY);
	double LeftMotor(double &invMaxValueXPlusY);
	double RightMotor(double &invMaxValueXPlusY);
	void compareEncoders();
	void breakTime();
	void setPower();
	void childProofShift();
	bool getIsTurning();
	
	//double getRightEncoderPulses() {return rightEncoder->GetRaw();}
	//double getLeftEncoderPulses() {return leftEncoder->GetRaw();}
	//double getRightEncoderDistance() {return rightEncoder->GetDistance();}
	//double getLeftEncoderDistance() {return leftEncoder->GetDistance();}
	void setCoasting(double newCoasting) {coasting = newCoasting;}
	double getLeftPow() {return leftPow;}
	double getRightPow() {return rightPow;}
	double getRatio() {return ratio;}
	bool getIsHighGear() {return isHighGear;}
	bool getIsLeftFaster() {return isLeftFaster;}
	double getLeftSpeed() {return leftSpeed;}
	double getRightSpeed() {return rightSpeed;}
	bool getIsDoneDriving();
	void turnAngle();
	void setSpeedPositive();
	void setGearLow();
	void setAngle(double angle);
	void TestLoop();

protected:
	bool isDoneDriving;
	void shift(); //moved to protected to prevent people from accidentally calling it in Robot.cpp
	bool isDownShifting;
	double leftPow;
	double rightPow;
	double maxLeftEncoderRate;
	double maxRightEncoderRate;
	double ratio;
	//double rightEncoderFix;
	//double leftEncoderFix;
	bool isHighGear; //Robot starts in low gear
	bool isLeftFaster;
	double leftSpeed;
	double rightSpeed;
	bool previousTriggerPressed; //what the trigger value was before the current press, allows for trigger to stay pressed w/o flipping
	double previousX;
	double previousY;
	double leftPosition;
	double rightPosition;
	double coasting;
	bool isTurning;
	double angle;

	OperatorInputs *operatorInputs;
	DriverStation *driverstation;
	CANTalon *leftTalons;
	CANTalon *rightTalons;
	CANTalon *leftTalons1;
	CANTalon *rightTalons1;
	Solenoid *gearShift;
	//Encoder *leftEncoder;
	//Encoder *rightEncoder;
	Timer *timer;
	Timer *timer1;
//	ADXRS450_Gyro *gyro;
	double prevGyro;
};



#endif /* SRC_DRIVETRAIN_H_ */
