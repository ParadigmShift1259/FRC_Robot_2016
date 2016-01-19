#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_


#include "WPILib.h"
#include <SpeedController.h>
#include "OperatorInputs.h"
#include <driverstation.h>

class Drivetrain
{
public:
	Drivetrain(OperatorInputs *inputs, DriverStation *ds);
	~Drivetrain();
	void rampLeftPower(double desiredPow, double rampSpeed);
	void rampRightPower(double desiredPow, double rampSpeed);
	void resetEncoders();
	double getRightPulses();
	double getLeftPulses();
	double getRightEncoderDistance();
	double getLeftEncoderDistance();
	double fix(double v);
	double LeftMotor();
	double RightMotor();
	void compareEncoders();
	void breakTime();
	void setPower();
	void shift();
	void setSpeedPositive();
	void childProofing();
	void setCoasting(double newCoasting);

protected:
	int Encoder_Top_Speed;
	int LEFT_PORT;
	int SECOND_LEFT_PORT;
	int RIGHT_PORT;
	int SECOND_RIGHT_PORT;
	int SHIFT_PORT_LOW;
	int SHIFT_PORT_HIGH;
	int SHIFT_MODULE;
	double joyStickX;
	double joyStickY;
	double leftPow;
	double rightPow;
	double totalSpeed;
	long sleeptime;
	double speedMult;
	double fixNum;
	double maxLeftEncoderRate;
	double maxRightEncoderRate;
	double ratio;
	double rightEncoderFix;
	double leftEncoderFix;
	long sleepTime;
	bool isHighGear; //Robot starts in low gear
	bool nemo;
	bool isLeftHigher;
	double leftSpeed;
	double rightSpeed;
	double encoderDeadzone;
	double encoderWaitTime;
	double leftChildProofSetter;
	double rightChildProofSetter;
	bool childProofConfirmed;
	double DISTANCE_PER_PULSE;
	bool previousTriggerPressed; //what the trigger value was before the current press, allows for trigger to stay pressed w/o flipping
	double previousLeftPow;
	double previousRightPow;
	double coasting;
	double teleopRamp;

	OperatorInputs *operatorInputs;
	DriverStation *driverstation;
	CANTalon *leftTalons;
	CANTalon *rightTalons;
	CANTalon *leftTalons1;
	CANTalon *rightTalons1;
	Solenoid *gearShift;
	Encoder *leftEncoder;
	Encoder *rightEncoder;
	Timer *timer;

};


#endif /* SRC_DRIVETRAIN_H_ */
