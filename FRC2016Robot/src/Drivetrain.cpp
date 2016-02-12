//  drivetrain.cpp


#include "Drivetrain.h"
#include "smartdashboard/smartdashboard.h"
#include <Timer.h>
#include <Talon.h>
#include <CanTalon.h>
#include <Encoder.h>
#include "Const.h"
#include <cmath>


using namespace std;


Drivetrain::Drivetrain(OperatorInputs *inputs, DriverStation *ds)
{
	angle = 0;
	isTurning = false;
	isDoneDriving = false;
	operatorInputs = inputs;
	driverstation = ds;
	prevGyro = 0.0;
	leftPow = 0;
	rightPow = 0;
	maxLeftEncoderRate = 0;
	maxRightEncoderRate = 0;
	ratio = 1;
	isHighGear = false; 			//Robot starts in low gear
	isLeftFaster = true;
	leftSpeed = 0;
	rightSpeed = 0;
	leftPosition = 0;
	rightPosition = 0;
	
	previousTriggerPressed = false; //what the trigger value was before the current press, allows for trigger to stay pressed w/o flipping
	previousX = 0;
	previousY = 0;
	coasting = 1;

	leftTalons = new CANTalon(LEFT_PORT);
	leftTalons1 = new CANTalon(SECOND_LEFT_PORT);
	//set leftTalons1 to follow leftTalons
	leftTalons1->SetControlMode(CANSpeedController::ControlMode::kFollower);
	leftTalons1->Set(LEFT_PORT);
	rightTalons = new CANTalon(RIGHT_PORT);
	rightTalons1 = new CANTalon(SECOND_RIGHT_PORT);
	//set rightTalons1 to follow rightTalons
	rightTalons1->SetControlMode(CANSpeedController::ControlMode::kFollower);
	rightTalons1->Set(RIGHT_PORT);
	gearShift = new Solenoid(SHIFT_MODULE, SHIFT_PORT_LOW);
	//gyro = new ADXRS450_Gyro();
	//gyro->Calibrate();
	//gyro->Reset();
	//Setup Encoders
	//leftEncoder = new Encoder(3, 4);
	//rightEncoder = new Encoder(5, 6);
	//leftEncoderFix = 0;
	//rightEncoderFix = 0;
	timer = new Timer();
	timer1 = new Timer();
	//Start all wheels off
	leftTalons->Set(0);
	//leftTalons1->Set(0);
	rightTalons->Set(0);
	//rightTalons1->Set(0);

	leftTalons->SetFeedbackDevice(CANTalon::QuadEncoder);
	//leftTalons1->SetFeedbackDevice(CANTalon::QuadEncoder);
	rightTalons->SetFeedbackDevice(CANTalon::QuadEncoder);
	//rightTalons1->SetFeedbackDevice(CANTalon::QuadEncoder);
	leftTalons->ConfigEncoderCodesPerRev(1024);
	//leftTalons1->ConfigEncoderCodesPerRev(1024);
	rightTalons->ConfigEncoderCodesPerRev(1024);
	//rightTalons1->ConfigEncoderCodesPerRev(1024);

	//Starts in low gear
	gearShift->Set(FLIP_HIGH_GEAR ^ isHighGear);
	//leftEncoder->SetDistancePerPulse(-DISTANCE_PER_PULSE);
	//rightEncoder->SetDistancePerPulse(DISTANCE_PER_PULSE);

	isDownShifting = false;
	invertLeft = -1;
	invertRight = 1;
}


Drivetrain::~Drivetrain()
{
	delete leftTalons;
	delete leftTalons1;
	delete rightTalons;
	delete rightTalons1;
	delete gearShift;
	//delete leftEncoder;
	//delete rightEncoder;
	delete timer;
}


void Drivetrain::Init()
{
	leftPow = 0;
	rightPow = 0;
	maxLeftEncoderRate = 0;
	maxRightEncoderRate = 0;
	ratio = 1;
	isLeftFaster = true;
	leftSpeed = 0;
	rightSpeed = 0;
	leftPosition = 0;
	rightPosition = 0;
	previousTriggerPressed = false;
	previousX = 0;
	previousY = 0;
	coasting = 1;
	angle = 0;
	leftTalons->SetPosition(0);
	rightTalons->SetPosition(0);
	timer1->Reset();
}

bool Drivetrain::getIsDoneDriving()
{
	return isDoneDriving;
}

bool Drivetrain::getIsTurning()
{
	return isTurning;
}

void Drivetrain::setAngle(double angle1)
{
	//angle = angle1+gyro->GetAngle();
	isTurning = false;
}

/**void Drivetrain::turnAngle()
{
	if(angle<gyro->GetAngle())
	{
		isTurning = true;
		double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
		double batteryRamping = RAMPING_RATE*invBatteryVoltage;
		rampLeftPower(0,batteryRamping);
		rampRightPower(0.5,batteryRamping);
	}
	if(angle>gyro->GetAngle())
	{
		isTurning = true;
		double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
		double batteryRamping = RAMPING_RATE*invBatteryVoltage;
		rampLeftPower(0.5,batteryRamping);
		rampRightPower(0,batteryRamping);
	}
	if(abs(gyro->GetAngle()-angle)<5)
	{
		isTurning = false;
	}

}**/

void Drivetrain::driveDistance(double distance)
{
	double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
	double batteryRamping = RAMPING_RATE_MIN*invBatteryVoltage;
	if(timer1->Get() < distance)
	{
		isDoneDriving = false;
		timer1->Start();

		previousX = rampInput(previousX,0.5,batteryRamping,batteryRamping);
		//previousY = rampInput(previousY,0.5,batteryRamping,batteryRamping);
		leftTalons->Set(invertLeft * previousX* MOTOR_SCALING);
		rightTalons->Set(invertRight * previousX* MOTOR_SCALING);
	} else if(timer1->Get() >= distance)
	{
		previousX = rampInput(previousX,0,batteryRamping,RAMPING_RATE_MAX*invBatteryVoltage);
		//previousY = rampInput(previousY,0,batteryRamping,batteryRamping);
		leftTalons->Set(invertLeft * previousX* MOTOR_SCALING);
		rightTalons->Set(invertRight * previousX* MOTOR_SCALING);
		if(previousX == 0) isDoneDriving = true;
	}
}

void Drivetrain::childProofShift()
{
	//current setting is start in low gear

	bool triggerPressed = operatorInputs->joystickTriggerPressed()||operatorInputs->xBoxLeftTrigger();
	if (triggerPressed && !previousTriggerPressed)
	{
		if(isHighGear)
		{
			isDownShifting = true;
			//SmartDashboard::PutNumber("isDownShifting", isDownShifting);
		}
		else
		{
			shift();
			isDownShifting = false;
			//SmartDashboard::PutNumber("isDownShifting", isDownShifting);
		}
	}
	if (isDownShifting && abs(previousX) < ENCODER_TOP_SPEED && abs(previousY) < ENCODER_TOP_SPEED)
	{
		shift();
		isDownShifting = false;
		//SmartDashboard::PutNumber("isDownShifting", isDownShifting);
	}
	previousTriggerPressed = triggerPressed;
}


//Sets the motors to coasting mode, shifts, and then sets them back to break mode
void Drivetrain::shift()
{
	leftTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	leftTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	rightTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	rightTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	isHighGear = !isHighGear;
	gearShift->Set(FLIP_HIGH_GEAR ^ isHighGear);
	leftTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	leftTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	rightTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	rightTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
}


void Drivetrain::setPower() 
{
	double joyStickX;
	double joyStickY;
	double invMaxValueXPlusY;
	joyStickX = operatorInputs->joystickX()+operatorInputs->xboxLeftX();
	if (isDownShifting)
	{
		joyStickY=0;
	}
	else
	{
		joyStickY = operatorInputs->joystickY()+operatorInputs->xboxLeftY();
	}
	//set fixnum = the maxiumum value for this angle on the joystick
	if (joyStickX == 0 || joyStickY == 0) 
	{
		invMaxValueXPlusY = 1;
	}
	else
	{
		if (abs(joyStickX) > abs(joyStickY))
		{
			double invMaxValueXPlusYMult = 1 / abs(joyStickX);
			invMaxValueXPlusY = abs(joyStickY) * invMaxValueXPlusYMult + 1;
			//Invert for later use
			invMaxValueXPlusY = 1 / invMaxValueXPlusY;
		}
		else
		{
			double invMaxValueXPlusYMult = 1 / abs(joyStickY);
			invMaxValueXPlusY = abs(joyStickX) * invMaxValueXPlusYMult + 1;
			//Invert for later use
			invMaxValueXPlusY = 1 / invMaxValueXPlusY;
		}
	}
	double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
	double BatteryRampingMin = RAMPING_RATE_MIN*invBatteryVoltage;
	double BatteryRampingMax = RAMPING_RATE_MAX*invBatteryVoltage;
	previousX = joyStickX;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax); //Left Motors are forward=negative
	previousY = rampInput(previousY, joyStickY, BatteryRampingMin, BatteryRampingMax); //Right Motors are forward=positive
	leftPow = previousY * Y_SCALING - previousX * X_SCALING;
	rightPow = previousY * Y_SCALING + previousX * X_SCALING;
	leftSpeed = leftTalons->GetSpeed();
	rightSpeed = rightTalons->GetSpeed();
	leftPosition = leftTalons->GetPosition();
	rightPosition = rightTalons->GetPosition();


	leftTalons->Set(invertLeft * coasting * LeftMotor(invMaxValueXPlusY) * MOTOR_SCALING);
	rightTalons->Set(invertRight * coasting * RightMotor(invMaxValueXPlusY) * MOTOR_SCALING);
	SmartDashboard::PutNumber("TurningRamp", previousX); //Left Motors are forward=negative
	SmartDashboard::PutNumber("LeftPow", invertLeft*leftPow); //Left Motors are forward=negative
	SmartDashboard::PutNumber("DrivingRamp", previousY); //Right Motors are forward=positive
	SmartDashboard::PutNumber("RightPow", invertRight*rightPow); //Right Motors are forward=positive
	SmartDashboard::PutString("Gear", isHighGear ? "High" : "Low");

	SmartDashboard::PutNumber("leftSpeed", leftSpeed);
	SmartDashboard::PutNumber("leftPosition", leftPosition);
	SmartDashboard::PutNumber("rightSpeed", rightSpeed);
	SmartDashboard::PutNumber("rightPosition", rightPosition);
}


double Drivetrain::rampInput(double previousPow, double desiredPow, double rampSpeedMin, double rampSpeedMax)
{
	double newPow = previousPow;
	double deltaPow = abs(desiredPow - previousPow);
	//Makes it so that robot can't go stop to full
	if (deltaPow < rampSpeedMin)
	{
		newPow = desiredPow;
	}
	else
	if (previousPow < desiredPow)
	{
		newPow += deltaPow*(rampSpeedMax-rampSpeedMin) + rampSpeedMin;
	}
	else
	if (previousPow > desiredPow)
	{
		newPow -= deltaPow*(rampSpeedMax-rampSpeedMin) + rampSpeedMin;
	}
	//leftTalons1->Set(-previousLeftPow);
	return newPow;
}


/*void Drivetrain::rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax)
{
	//Makes it so that robot can't go stop to full
	if (abs(desiredPow - previousRightPow) < rampSpeedMin)
	{
		previousRightPow = desiredPow;
	}
	else
	if (previousRightPow < desiredPow)
	{
		previousRightPow += rampSpeedMin;
	}
	else
	if (previousRightPow > desiredPow)
	{
		previousRightPow -= rampSpeedMin;
	}
	rightTalons->Set(previousRightPow);
	//rightTalons1->Set(previousRightPow);
}*/


double Drivetrain::LeftMotor(double &invMaxValueXPlusY)
{
	double fixLeftPow = fix(leftPow, invMaxValueXPlusY);
	//moved rightSpeed to class scope, it is being set in setPower()

	if (leftPow != 0 && rightPow != 0)
	{
		maxLeftEncoderRate = abs(leftSpeed / leftPow);
		if (min(abs(leftSpeed), abs(rightSpeed)) > ENCODER_TOP_SPEED)
		{
			breakTime();
		}
		if (isLeftFaster)
		{
			fixLeftPow = ratio * fixLeftPow;
		}
	}
	return (fixLeftPow);
}


double Drivetrain::RightMotor(double &invMaxValueXPlusY)
{
	//moved rightSpeed to class scope, it is being set in setPower()

	double fixRightPow = fix(rightPow, invMaxValueXPlusY);

	if (leftPow != 0 && rightPow != 0)
	{
		maxRightEncoderRate = abs(rightSpeed / rightPow);
		if (min(abs(leftSpeed), abs(rightSpeed)) > ENCODER_TOP_SPEED)
		{
			breakTime();
		}
		if (!isLeftFaster)
		{
			fixRightPow = ratio * fixRightPow;
		}
	}
	return (fixRightPow);
}


void Drivetrain::compareEncoders() 
{
	//If left motor speed is bigger than the right motor speed return true, else false

	if (maxRightEncoderRate > maxLeftEncoderRate)
	{
		ratio = maxLeftEncoderRate / maxRightEncoderRate;
		//leftEncoderFix = maxRightEncoderRate * ratio;
		isLeftFaster = false;
	}
	else
	if (maxLeftEncoderRate > maxRightEncoderRate)
	{
		ratio = maxRightEncoderRate / maxLeftEncoderRate;
		//rightEncoderFix = maxLeftEncoderRate * ratio;
		isLeftFaster = true;
	}
	else
	{
		ratio = 1;
	}
}


/*void Drivetrain::resetEncoders()
{ //Resets current raw encoder value to 0
	leftEncoder->Reset();
	rightEncoder->Reset();
	gearShift->Set(!(FLIP_HIGH_GEAR^isHighGear));
}*/


double Drivetrain::fix(double v, double &invMaxValueXPlusY)
{
	return v * invMaxValueXPlusY;
}


void Drivetrain::breakTime()
{
	SmartDashboard::PutNumber("Ratio", ratio);
	SmartDashboard::PutBoolean("Left > Right", isLeftFaster);
	SmartDashboard::PutNumber("Timer time", timer->Get());
	if (timer->Get() > ENCODER_WAIT_TIME)
	{
		compareEncoders();
		timer->Reset();
	}
}


void Drivetrain::setGearLow()
{
	isHighGear=false;
	gearShift->Set(true ^ isHighGear);
}


void Drivetrain::TestLoop()
{
	double joyStickX;
	double joyStickY;
	joyStickX = operatorInputs->joystickX()+operatorInputs->xboxLeftX();
	joyStickY = operatorInputs->joystickY()+operatorInputs->xboxLeftY();
	leftPow = (-joyStickY + joyStickX+leftPow)*0.5;
	rightPow = (-joyStickY - joyStickX+rightPow)*0.5;
	leftPow = leftPow >1 ? 1 : (leftPow < -1 ? -1 : leftPow);
	rightPow = rightPow >1 ? 1 : (rightPow < -1 ? -1 : rightPow);
	leftTalons->Set(leftPow);
	rightTalons->Set(rightPow);
	leftSpeed = leftTalons->GetSpeed();
	leftPosition = leftTalons->GetPosition();
	rightSpeed = rightTalons->GetSpeed();
	rightPosition = rightTalons->GetPosition();
	SmartDashboard::PutNumber("leftPow", leftPow);
	SmartDashboard::PutNumber("leftSpeed", leftSpeed);
	SmartDashboard::PutNumber("leftPosition", leftPosition);
	SmartDashboard::PutNumber("rightPow", rightPow);
	SmartDashboard::PutNumber("rightSpeed", rightSpeed);
	SmartDashboard::PutNumber("rightPosition", rightPosition);
}

void Drivetrain::ChangeDirection()
{
	invertRight = invertRight*(-1);
	invertLeft = invertLeft*(-1);
}
