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
	m_righttalonlead = new CANTalon(CAN_RIGHT_PORT);
	m_righttalonfollow = new CANTalon(CAN_SECOND_RIGHT_PORT);

	m_lefttalonlead->SetControlMode(CANTalon::kPercentVbus);
	m_lefttalonlead->Set(0);
	m_lefttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);
	m_lefttalonlead->ConfigEncoderCodesPerRev(1024);
	m_lefttalonlead->SetSensorDirection(true);
	m_lefttalonlead->SetPosition(0);

	m_righttalonlead->SetControlMode(CANTalon::kPercentVbus);
	m_righttalonlead->Set(0);
	m_righttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);
	m_righttalonlead->ConfigEncoderCodesPerRev(1024);
	m_righttalonlead->SetSensorDirection(true);
	m_righttalonlead->SetPosition(0);

	m_lefttalonfollow->SetControlMode(CANSpeedController::ControlMode::kFollower);
	m_lefttalonfollow->Set(CAN_LEFT_PORT);
	m_righttalonfollow->SetControlMode(CANSpeedController::ControlMode::kFollower);
	m_righttalonfollow->Set(CAN_RIGHT_PORT);

	m_lefttalonlead->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_lefttalonfollow->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonlead->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonfollow->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);

	//Setup Encoders
	//m_leftEncoder = new Encoder(3, 4);
	//m_rightEncoder = new Encoder(5, 6);
	//m_leftEncoder->SetDistancePerPulse(-CAN_DISTANCE_PER_PULSE);
	//m_rightEncoder->SetDistancePerPulse(CAN_DISTANCE_PER_PULSE);
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
	m_lowspeedmode = false;
	m_shift = false;

	m_leftpow = 0;
	m_rightpow = 0;
	m_ratiolr = 1;
	m_isleftfaster = true;
	m_previousx = 0;
	m_previousy = 0;
	m_coasting = 1;
	m_invertleft = INVERT_LEFT;
	m_invertright = INVERT_RIGHT;
	m_direction = DT_DEFAULT_DIRECTION;

	m_timerencoder = new Timer();
}


Drivetrain::~Drivetrain()
{
	delete m_lefttalonlead;
	delete m_lefttalonfollow;
	delete m_righttalonlead;
	delete m_righttalonfollow;
	//delete m_leftEncoder;
	//delete m_rightEncoder;
	delete m_shifter;
	delete m_timerencoder;
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
	m_lefttalonlead->Set(0);
	//m_lefttalonfollow->Set(0);
	m_righttalonlead->Set(0);
	//m_righttalonfollow->Set(0);
	m_timerencoder->Reset();
	m_ishighgear = false;
	// Starts in low gear
	m_shifter->Set(FLIP_HIGH_GEAR ^ m_ishighgear);
	m_isdownshifting = false;
	m_lowspeedmode = false;
	m_shift = false;
}


void Drivetrain::Loop()
{
	static unsigned int loopcnt = 0;
	static unsigned int shiftcnt = 0;
	double x;
	double y;

	if (m_inputs->xBoxLeftTrigger())
	{
		m_shift = true;
		m_lowspeedmode = false;
	}

	LowSpeedDriving();

	x = m_inputs->xBoxLeftX();
	y = m_inputs->xBoxLeftY();

	if (m_isdownshifting)
		y = 0;

	if (m_lowspeedmode)
	{
		x = x * LOWSPEED_MODIFIER_X;
		y = y * LOWSPEED_MODIFIER_Y;
	}

	Drive(x, y, true);

	if (m_shift)
	{
		shiftcnt += 4;
		if (m_ishighgear)
		{
			m_isdownshifting = true;
			shiftcnt += 2;
		}
		else
		{
			Shift();
			m_isdownshifting = false;
			shiftcnt += 1;
		}
	}

	if (m_isdownshifting && (abs(m_previousx * X_SCALING) < ENCODER_TOP_SPEED) && (abs(m_previousy * Y_SCALING) < ENCODER_TOP_SPEED))
	{
		loopcnt++;
		Shift();
		m_isdownshifting = false;
	}

	SmartDashboard::PutNumber("DT01_x", x);
	SmartDashboard::PutNumber("DT02_y", y);
	SmartDashboard::PutNumber("DT03_top", ENCODER_TOP_SPEED);
	SmartDashboard::PutNumber("DT04_loop_count", loopcnt);
	SmartDashboard::PutNumber("DT05_shift", m_shift);
	SmartDashboard::PutNumber("DT06_shift_count", shiftcnt);
	SmartDashboard::PutNumber("DT07_shift_down", m_isdownshifting);
	SmartDashboard::PutNumber("DT08_abs_x", (abs(m_previousx * X_SCALING) < ENCODER_TOP_SPEED));
	SmartDashboard::PutNumber("DT09_abs_y", (abs(m_previousy * Y_SCALING) < ENCODER_TOP_SPEED));
}


void Drivetrain::Stop()
{
	m_ishighgear = false;
	m_shifter->Set(true ^ m_ishighgear);
}


void Drivetrain::Drive(double x, double y, bool ramp)
{
	double yd = y * m_direction;
	double maxpower;

	if (x == 0 || yd == 0)
	{
		maxpower = 1;
	}
	else
	{
		if (abs(x) > abs(yd))
			maxpower = (abs(yd) / abs(x)) + 1;
		else
			maxpower = (abs(x) / abs(yd)) + 1;
	}
	if (!ramp)
	{
		m_previousx = x;	//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax);
		m_previousy = yd;
		m_leftpow = m_previousy - m_previousx;
		m_rightpow = m_previousy + m_previousx;
	}
	else
	{
		double battery = m_driverstation->GetInstance().GetBatteryVoltage();
		double rampmin = RAMPING_RATE_MIN / battery;
		double rampmax = RAMPING_RATE_MAX / battery;
		SmartDashboard::PutNumber("DT10_battery", battery);
		m_previousx = x;	//rampInput(previousX, joyStickX, rampmin, rampmax);
		m_previousy = Ramp(m_previousy, yd, rampmin, rampmax);
		m_leftpow = m_previousy * Y_SCALING - (m_previousx * X_SCALING);
		m_rightpow = m_previousy * Y_SCALING + (m_previousx * X_SCALING);
	}
	m_leftspeed = m_lefttalonlead->GetSpeed();
	m_rightspeed = m_righttalonlead->GetSpeed();
	m_leftposition = m_lefttalonlead->GetPosition();
	m_rightposition = m_righttalonlead->GetPosition();

	m_lefttalonlead->Set(m_invertleft * m_coasting * LeftMotor(maxpower));
	//m_lefttalonfollow->Set(m_invertleft * m_coasting * LeftMotor(maxpower));
	m_righttalonlead->Set(m_invertright * m_coasting * RightMotor(maxpower));
	//m_righttalonfollow->Set(m_invertright * m_coasting * RightMotor(maxpower));

	SmartDashboard::PutNumber("DT11_turningramp", m_previousx); 			//Left Motors are forward=negative
	SmartDashboard::PutNumber("DT12_drivingramp", m_previousy); 			//Right Motors are forward=positive
	SmartDashboard::PutNumber("DT13_leftpow", m_invertleft*m_leftpow); 		//Left Motors are forward=negative
	SmartDashboard::PutNumber("DT14_rightpow", m_invertright*m_rightpow); 	//Right Motors are forward=positive
	SmartDashboard::PutNumber("DT15_gear", m_ishighgear);
	SmartDashboard::PutNumber("DT16_leftspeed", m_leftspeed);
	SmartDashboard::PutNumber("DT17_rightspeed", m_rightspeed);
	SmartDashboard::PutNumber("DT18_leftposition", m_leftposition);
	SmartDashboard::PutNumber("DT19_rightposition", m_rightposition);
}


// sets the motors to coasting mode, shifts, and then sets them back to break mode
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
	m_shift = false;
}


// change direction and return true if going forward
bool Drivetrain::ChangeDirection()
{
	m_direction *= -1.0;
	return (m_direction == DT_DEFAULT_DIRECTION);
}


void Drivetrain::LowSpeedDriving()
{
	SmartDashboard::PutNumber("DT16_lowspeed", m_lowspeedmode);
	if (m_inputs->button10())
	{
		m_lowspeedmode = !m_lowspeedmode;
		if (m_ishighgear && m_lowspeedmode)
		{
			m_shift = true;
		}
	}
}


// ramp the power
double Drivetrain::Ramp(double previous, double desired, double rampmin, double rampmax)
{
	double newpow = previous;
	double delta = abs(desired - previous);

	// Makes it so that robot can't go stop to full
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
	//moved rightSpeed to class scope, it is being set in setPower()

	double leftpow = m_leftpow * LEFT_MOTOR_SCALING / maxpower;

	/*if (m_leftpow != 0 && m_rightpow != 0)
	{
		m_leftencodermax = abs(m_leftspeed / m_leftpow);
		if (min(abs(m_leftspeed), abs(m_rightspeed)) > ENCODER_TOP_SPEED)
			CheckEncoderTimer();
		if (m_isleftfaster)
			leftpow = m_ratiolr * leftpow;
	}*/
	return leftpow;
}


double Drivetrain::RightMotor(double &maxpower)
{
	//moved rightSpeed to class scope, it is being set in setPower()

	double rightpow = m_rightpow * RIGHT_MOTOR_SCALING / maxpower;

	/*if (m_leftpow != 0 && m_rightpow != 0)
	{
		m_rightencodermax = abs(m_rightspeed / m_rightpow);
		if (min(abs(m_leftspeed), abs(m_rightspeed)) > ENCODER_TOP_SPEED)
			CheckEncoderTimer();
		if (!m_isleftfaster)
			rightpow = m_ratiolr * rightpow;
	}*/
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


void Drivetrain::ResetEncoders()
{
	//leftEncoder->Reset();
	//rightEncoder->Reset();
}


void Drivetrain::CheckEncoderTimer()
{
	//SmartDashboard::PutNumber("Ratio", m_ratiolr);
	//SmartDashboard::PutBoolean("Left > Right", m_isleftfaster);
	//SmartDashboard::PutNumber("Timer time", m_timerencoder->Get());
	if (m_timerencoder->Get() > ENCODER_WAIT_TIME)
	{
		SetRatioLR();
		m_timerencoder->Reset();
	}
}
