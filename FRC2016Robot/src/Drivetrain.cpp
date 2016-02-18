//  drivetrain.cpp


#include "Drivetrain.h"
#include "Const.h"
#include "smartdashboard/smartdashboard.h"
#include <Timer.h>
#include <Talon.h>
#include <CanTalon.h>
#include <Encoder.h>
#include <cmath>


using namespace std;


Drivetrain::Drivetrain(OperatorInputs *inputs, DriverStation *ds)
{
	m_inputs = inputs;
	m_driverstation = ds;

	m_lefttalonlead = new CANTalon(CAN_LEFT_PORT);
	m_lefttalonfollow = new CANTalon(CAN_SECOND_LEFT_PORT);
	// set leftTalons1 to follow leftTalons
	m_lefttalonfollow->SetControlMode(CANSpeedController::ControlMode::kFollower);
	m_lefttalonfollow->Set(CAN_LEFT_PORT);

	m_righttalonlead = new CANTalon(CAN_RIGHT_PORT);
	m_righttalonfollow = new CANTalon(CAN_SECOND_RIGHT_PORT);
	// set rightTalons1 to follow rightTalons
	m_righttalonfollow->SetControlMode(CANSpeedController::ControlMode::kFollower);
	m_righttalonfollow->Set(CAN_RIGHT_PORT);

	m_lefttalonlead->Set(0);
	m_lefttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);
	m_lefttalonlead->ConfigEncoderCodesPerRev(1024);
	m_righttalonlead->Set(0);
	m_righttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);
	m_righttalonlead->ConfigEncoderCodesPerRev(1024);

	//Setup Encoders
	//leftEncoder = new Encoder(3, 4);
	//rightEncoder = new Encoder(5, 6);
	//leftEncoder->SetDistancePerPulse(-CAN_DISTANCE_PER_PULSE);
	//rightEncoder->SetDistancePerPulse(CAN_DISTANCE_PER_PULSE);
	m_leftencoderfix = 0;
	m_rightencoderfix = 0;
	m_leftencodermax = 0;
	m_rightencodermax = 0;
	m_leftspeed = 0;
	m_rightspeed = 0;
	m_leftposition = 0;
	m_rightposition = 0;

	m_shifter = new Solenoid(PCM_SHIFT_MODULE, PCM_SHIFT_PORT_LOW);
	// Robot starts in low gear
	m_ishighgear = false;
	// Starts in low gear
	m_shifter->Set(FLIP_HIGH_GEAR ^ m_ishighgear);
	m_isdownshifting = false;

	m_leftpow = 0;
	m_rightpow = 0;
	m_ratiolr = 1;
	m_isleftfaster = true;
	m_previousx = 0;
	m_previousy = 0;
	m_coasting = 1;
	m_invertleft = INVERT_LEFT;
	m_invertright = INVERT_RIGHT;
	m_direction = 1.0;

	//gyro = new ADXRS450_Gyro();
	//gyro->Calibrate();
	//gyro->Reset();
	m_timerencoder = new Timer();
	m_timerautonomous = new Timer();
	m_angle = 0;
	m_isturning = false;
	m_isdonedriving = false;
	m_prevgyro = 0.0;
}


Drivetrain::~Drivetrain()
{
	delete m_lefttalonlead;
	delete m_lefttalonfollow;
	delete m_righttalonlead;
	delete m_righttalonfollow;
	//delete leftEncoder;
	//delete rightEncoder;
	delete m_shifter;
	delete m_timerencoder;
	delete m_timerautonomous;
}


void Drivetrain::Init()
{
	m_leftpow = 0;
	m_rightpow = 0;
	m_leftencodermax = 0;
	m_rightencodermax = 0;
	m_ratiolr = 1;
	m_isleftfaster = true;
	m_leftspeed = 0;
	m_rightspeed = 0;
	m_leftposition = 0;
	m_rightposition = 0;
	m_previousx = 0;
	m_previousy = 0;
	m_coasting = 1;
	m_angle = 0;
	m_lefttalonlead->Set(0);
	m_righttalonlead->Set(0);
	m_timerencoder->Reset();
	m_timerautonomous->Reset();
}


void Drivetrain::Loop()
{
	double x;
	double y;

	x = m_inputs->joystickX()+m_inputs->xBoxLeftX();

	if (m_isdownshifting)
		y=0;
	else
	{
		y = m_inputs->joystickY()+m_inputs->xBoxLeftY();
		y *= m_direction;
	}

	Drive(x, y, true);

	if (m_inputs->joystickTrigger() || m_inputs->xBoxLeftTrigger())
	{
		if (m_ishighgear)
			m_isdownshifting = true;
		else
			Shift();
			m_isdownshifting = false;
	}
	if (m_isdownshifting && (abs(m_previousx) < ENCODER_TOP_SPEED) && (abs(m_previousy) < ENCODER_TOP_SPEED))
	{
		Shift();
		m_isdownshifting = false;
	}
}


void Drivetrain::Drive(double x, double y, bool ramp)
{
	double maxpower;
	if (x == 0 || y == 0) 
	{
		maxpower = 1;
	}
	else
	{
		if (abs(x) > abs(y))
			maxpower = (abs(y) / abs(x)) + 1;
		else
			maxpower = (abs(x) / abs(y)) + 1;
	}
	if (!ramp)
	{
		m_previousx = x;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax);
		m_previousy = y;
		m_leftpow = m_previousy - m_previousx;
		m_rightpow = m_previousy + m_previousx;
	}
	if (ramp)
	{
		double rampmin = RAMPING_RATE_MIN / m_driverstation->GetInstance().GetBatteryVoltage();
		double rampmax = RAMPING_RATE_MAX / m_driverstation->GetInstance().GetBatteryVoltage();
		m_previousx = x;//rampInput(previousX, joyStickX, rampmin, rampmax);
		m_previousy = Ramp(m_previousy, y, rampmin, rampmax);
		m_leftpow = m_previousy * Y_SCALING - m_previousx * X_SCALING;
		m_rightpow = m_previousy * Y_SCALING + m_previousx * X_SCALING;
	}
	m_leftspeed = m_lefttalonlead->GetSpeed();
	m_rightspeed = m_righttalonlead->GetSpeed();
	m_leftposition = m_lefttalonlead->GetPosition();
	m_rightposition = m_righttalonlead->GetPosition();

	m_lefttalonlead->Set(m_invertleft * m_coasting * LeftMotor(maxpower));
	m_righttalonlead->Set(m_invertright * m_coasting * RightMotor(maxpower));
	SmartDashboard::PutNumber("TurningRamp", m_previousx); //Left Motors are forward=negative
	SmartDashboard::PutNumber("LeftPow", m_invertleft*m_leftpow); //Left Motors are forward=negative
	SmartDashboard::PutNumber("DrivingRamp", m_previousy); //Right Motors are forward=positive
	SmartDashboard::PutNumber("RightPow", m_invertright*m_rightpow); //Right Motors are forward=positive
	SmartDashboard::PutString("Gear", m_ishighgear ? "High" : "Low");

	SmartDashboard::PutNumber("leftSpeed", m_leftspeed);
	SmartDashboard::PutNumber("leftPosition", m_leftposition);
	SmartDashboard::PutNumber("rightSpeed", m_rightspeed);
	SmartDashboard::PutNumber("rightPosition", m_rightposition);
}


/*void Drivetrain::DriveXY(double joyStickX, double joyStickY)
{
	double invMaxValueXPlusY;

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
	//double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
	//double BatteryRampingMin = RAMPING_RATE_MIN*invBatteryVoltage;
	//double BatteryRampingMax = RAMPING_RATE_MAX*invBatteryVoltage;
	m_previousx = joyStickX;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax); //Left Motors are forward=negative
	m_previousy = joyStickY;//rampInput(previousY, joyStickY, BatteryRampingMin, BatteryRampingMax); //Right Motors are forward=positive
	//leftPow = previousY * Y_SCALING - previousX * X_SCALING;
	m_leftpow = m_previousy - m_previousx;
	//rightPow = previousY * Y_SCALING + previousX * X_SCALING;
	m_rightpow = m_previousy + m_previousx;
	m_leftspeed = m_lefttalonlead->GetSpeed();
	m_rightspeed = m_righttalonlead->GetSpeed();
	m_leftposition = m_lefttalonlead->GetPosition();
	m_rightposition = m_righttalonlead->GetPosition();

	m_lefttalonlead->Set(m_invertleft * m_coasting * LeftMotor(invMaxValueXPlusY) * LEFT_MOTOR_SCALING);
	m_righttalonlead->Set(m_invertright * m_coasting * RightMotor(invMaxValueXPlusY) * RIGHT_MOTOR_SCALING);

	SmartDashboard::PutNumber("LeftPow", m_invertleft*m_leftpow); //Left Motors are forward=negative
	SmartDashboard::PutNumber("RightPow", m_invertright*m_rightpow); //Right Motors are forward=positive
}


void Drivetrain::setPowerXYleft(double joyStickX, double joyStickY)
{
	double invMaxValueXPlusY;

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
	//double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
	//double BatteryRampingMin = RAMPING_RATE_MIN*invBatteryVoltage;
	//double BatteryRampingMax = RAMPING_RATE_MAX*invBatteryVoltage;
	previousX = joyStickX;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax); //Left Motors are forward=negative
	previousY = joyStickY;//rampInput(previousY, joyStickY, BatteryRampingMin, BatteryRampingMax); //Right Motors are forward=positive
	//leftPow = previousY * Y_SCALING - previousX * X_SCALING;
	leftPow = previousY - previousX;
	//rightPow = previousY * Y_SCALING + previousX * X_SCALING;
	rightPow = previousY + previousX;
	leftSpeed = leftTalons->GetSpeed();
	rightSpeed = rightTalons->GetSpeed();
	leftPosition = leftTalons->GetPosition();
	rightPosition = rightTalons->GetPosition();

	leftTalons->Set(invertLeft * coasting * LeftMotor(invMaxValueXPlusY) * LEFT_MOTOR_SCALING);
	//rightTalons->Set(invertRight * coasting * RightMotor(invMaxValueXPlusY) * RIGHT_MOTOR_SCALING);
	rightTalons->Set(0);

	SmartDashboard::PutNumber("LeftPow", invertLeft*leftPow); //Left Motors are forward=negative
	SmartDashboard::PutNumber("RightPow", invertRight*rightPow); //Right Motors are forward=positive
}


void Drivetrain::setPowerXYright(double joyStickX, double joyStickY)
{
	double invMaxValueXPlusY;

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
	//double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
	//double BatteryRampingMin = RAMPING_RATE_MIN*invBatteryVoltage;
	//double BatteryRampingMax = RAMPING_RATE_MAX*invBatteryVoltage;
	previousX = joyStickX;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax); //Left Motors are forward=negative
	previousY = joyStickY;//rampInput(previousY, joyStickY, BatteryRampingMin, BatteryRampingMax); //Right Motors are forward=positive
	//leftPow = previousY * Y_SCALING - previousX * X_SCALING;
	leftPow = previousY - previousX;
	//rightPow = previousY * Y_SCALING + previousX * X_SCALING;
	rightPow = previousY + previousX;
	leftSpeed = leftTalons->GetSpeed();
	rightSpeed = rightTalons->GetSpeed();
	leftPosition = leftTalons->GetPosition();
	rightPosition = rightTalons->GetPosition();

	//leftTalons->Set(invertLeft * coasting * LeftMotor(invMaxValueXPlusY) * LEFT_MOTOR_SCALING);
	leftTalons->Set(0);
	rightTalons->Set(invertRight * coasting * RightMotor(invMaxValueXPlusY) * RIGHT_MOTOR_SCALING);

	SmartDashboard::PutNumber("LeftPow", invertLeft*leftPow); //Left Motors are forward=negative
	SmartDashboard::PutNumber("RightPow", invertRight*rightPow); //Right Motors are forward=positive
}*/


//Sets the motors to coasting mode, shifts, and then sets them back to break mode
void Drivetrain::Shift()
{
	m_lefttalonlead->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_lefttalonfollow->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_righttalonlead->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_righttalonfollow->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_ishighgear = !m_ishighgear;
	m_shifter->Set(FLIP_HIGH_GEAR ^ m_ishighgear);
	m_lefttalonlead->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_lefttalonfollow->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonlead->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonfollow->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
}


// change direction and return true if going forward
bool Drivetrain::ChangeDirection()
{
	m_direction *= -1.0;
	return (m_direction == 1.0);
}


double Drivetrain::Ramp(double previous, double desired, double rampmin, double rampmax)
{
	double newpow = previous;
	double delta = abs(desired - previous);
	//Makes it so that robot can't go stop to full
	if (delta <= rampmin)
		newpow = desired;
	else
	if (previous < desired)
		newpow += max((delta*rampmax), rampmin);
	else
	if (previous > desired)
		newpow -= max((delta*rampmax), rampmin);
	//leftTalons1->Set(-previousLeftPow);
	return newpow;
}


/*
void Drivetrain::rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax)
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
}
*/


double Drivetrain::LeftMotor(double &maxpower)
{
	double leftpow = m_leftpow * LEFT_MOTOR_SCALING / maxpower;
	//moved rightSpeed to class scope, it is being set in setPower()

	if (m_leftpow != 0 && m_rightpow != 0)
	{
		m_leftencodermax = abs(m_leftspeed / m_leftpow);
		if (min(abs(m_leftspeed), abs(m_rightspeed)) > ENCODER_TOP_SPEED)
			CheckEncoderTimer();
		if (m_isleftfaster)
			leftpow = m_ratiolr * leftpow;
	}
	return leftpow;
}


double Drivetrain::RightMotor(double &maxpower)
{
	//moved rightSpeed to class scope, it is being set in setPower()

	double rightpow = m_rightpow * RIGHT_MOTOR_SCALING / maxpower;

	if (m_leftpow != 0 && m_rightpow != 0)
	{
		m_rightencodermax = abs(m_rightspeed / m_rightpow);
		if (min(abs(m_leftspeed), abs(m_rightspeed)) > ENCODER_TOP_SPEED)
			CheckEncoderTimer();
		if (!m_isleftfaster)
			rightpow = m_ratiolr * rightpow;
	}
	return rightpow;
}


void Drivetrain::SetRatioLR()
{
	//If left motor speed is bigger than the right motor speed return true, else false

	if (m_rightencodermax > m_leftencodermax)
	{
		m_ratiolr = m_leftencodermax / m_rightencodermax;
		m_isleftfaster = false;
	}
	else
	if (m_leftencodermax > m_rightencodermax)
	{
		m_ratiolr = m_rightencodermax / m_leftencodermax;
		m_isleftfaster = true;
	}
	else
	{
		m_ratiolr = 1;
	}
}


/*
void Drivetrain::resetEncoders()
{
	leftEncoder->Reset();
	rightEncoder->Reset();
	gearShift->Set(!(FLIP_HIGH_GEAR^isHighGear));
}
*/


void Drivetrain::CheckEncoderTimer()
{
	SmartDashboard::PutNumber("Ratio", m_ratiolr);
	SmartDashboard::PutBoolean("Left > Right", m_isleftfaster);
	SmartDashboard::PutNumber("Timer time", m_timerencoder->Get());
	if (m_timerencoder->Get() > ENCODER_WAIT_TIME)
	{
		SetRatioLR();
		m_timerencoder->Reset();
	}
}


void Drivetrain::Stop()
{
	m_ishighgear = false;
	m_shifter->Set(true ^ m_ishighgear);
}


void Drivetrain::SetAngle(double angle)
{
	//angle = angle + m_gyro->GetAngle();
	m_isturning = false;
}


/*
void Drivetrain::turnAngle()
{
	if (m_angle < m_gyro->GetAngle())
	{
		m_isturning = true;
		double batteryramp = RAMPING_RATE / m_driverstation->GetInstance().GetBatteryVoltage();
		rampLeftPower(0,batteryramp);
		rampRightPower(0.5,batteryramp);
	}
	if (m_angle > m_gyro->GetAngle())
	{
		m_isturning = true;
		double batteryramp = RAMPING_RATE / m_driverstation->GetInstance().GetBatteryVoltage();
		rampLeftPower(0.5,batteryramp);
		rampRightPower(0,batteryramp);
	}
	if (abs(m_gyro->GetAngle() - m_angle) < 5)
	{
		m_isturning = false;
	}
}
*/


void Drivetrain::DriveDistance(double distance)
{
	double batteryramp = RAMPING_RATE_MIN / m_driverstation->GetInstance().GetBatteryVoltage();

	if (m_timerautonomous->Get() < distance)
	{
		m_isdonedriving = false;
		m_timerautonomous->Start();
		m_previousy = Ramp(m_previousx,0.5,batteryramp,batteryramp);
		//previousY = rampInput(previousY,0.5,batteryramp,batteryramp);
		m_lefttalonlead->Set(m_invertleft * LeftMotor(m_previousy));
		m_righttalonlead->Set(m_invertright * RightMotor(m_previousy));
	}
	else
	if (m_timerautonomous->Get() >= distance)
	{
		m_previousy = Ramp(m_previousx,0,batteryramp,RAMPING_RATE_MAX / m_driverstation->GetInstance().GetBatteryVoltage());
		//previousY = rampInput(previousY,0,batteryRamping,batteryRamping);
		m_lefttalonlead->Set(m_invertleft * LeftMotor(m_previousy));
		m_righttalonlead->Set(m_invertright * RightMotor(m_previousy));
		if (m_previousx == 0)
			m_isdonedriving = true;
	}
}
