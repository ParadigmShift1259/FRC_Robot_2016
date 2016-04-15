// autonomous.cpp


#include "Autonomous.h"
#include "const.h"
#include <driverstation.h>
#include <cmath>


Autonomous::Autonomous(DriverStation *driverstation, OperatorInputs *operatorinputs, Drivetrain *drivetrain, VisionTargeting *vision)
{
	m_driverstation = driverstation;
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_vision = vision;
	m_gyro = new AnalogGyro(ALG_AUTONOMOUS_GYRO);
	m_gyro->Calibrate();
	m_accel = new BuiltInAccelerometer(Accelerometer::Range::kRange_4G);
	m_stage = kIdle;
	m_counter = 0;
	m_maxspeed = -1.0;
	m_minspeed = -0.20;
	m_rate = 0.0;
	m_speed = 0.0;
	m_distance = 15.0;
}


Autonomous::~Autonomous()
{
}


void Autonomous::Init()
{
	m_gyro->Reset();
	if (m_driverstation->IsAutonomous())
		m_stage = kStart;
	else
		m_stage = kIdle;
	m_counter = 0;
	m_maxspeed = 1.0;
	m_minspeed = 0.20;
	m_rate = 0.0;
	m_speed = 0.0;
	m_distance = 15.0;
}


void Autonomous::Loop(int delay)
{
	double angle = m_gyro->GetAngle();
	SmartDashboard::PutNumber("AU1_gyro", angle);

	double xaccel = m_accel->GetX();
	double yaccel = m_accel->GetY();
	double zaccel = m_accel->GetZ();
	SmartDashboard::PutNumber("AU2_xaccel", xaccel);
	SmartDashboard::PutNumber("AU3_yaccel", yaccel);
	SmartDashboard::PutNumber("AU4_zaccel", zaccel);

	double leftposition = m_drivetrain->LeftTalon()->GetPosition();
	double rightposition = m_drivetrain->RightTalon()->GetPosition();
	SmartDashboard::PutNumber("AU5_leftpos", leftposition);
	SmartDashboard::PutNumber("AU6_rightpos", rightposition);

	string tempstr = SmartDashboard::GetString("DB/String 0", "0");
	double tempint = atof(tempstr.c_str());
	if (tempint != 0)
		m_distance = tempint;

	tempstr = SmartDashboard::GetString("DB/String 1", "0");
	tempint = atof(tempstr.c_str());
	if (tempint != 0)
		m_maxspeed = tempint;

	tempstr = SmartDashboard::GetString("DB/String 2", "0");
	tempint = atof(tempstr.c_str());
	if (tempint != 0)
		m_minspeed = tempint;

	SmartDashboard::PutNumber("AU7_distance", m_distance);
	SmartDashboard::PutNumber("AU8_maxspeed", m_maxspeed);
	SmartDashboard::PutNumber("AU9_minspeed", m_minspeed);

	if (m_stage == kIdle)
		if (!m_driverstation->IsAutonomous())
			return;

	switch (m_stage)
	{
	case kIdle:
		break;
	case kStart:
		DriverStation::ReportError("Auto Start");
		m_drivetrain->LeftTalon()->SetPosition(0);
		m_drivetrain->RightTalon()->SetPosition(0);
		m_counter = 50;
		m_speed = m_maxspeed;
		m_stage = kDrive;
		m_drivetrain->Drive(0, m_speed);
		break;
	case kDrive:
		if (m_counter > 0)
		{
			if ((m_counter % 5) == 0)
				DriverStation::ReportError("Auto Drive");
			m_counter--;
			if ((abs(m_drivetrain->LeftTalon()->GetPosition()) >= m_distance) &&
				(abs(m_drivetrain->RightTalon()->GetPosition()) >= m_distance))
				m_counter = 0;
		}
		if (m_counter <= 0)
		{
			m_stage = kStop;
		}
		m_drivetrain->Drive(0, m_speed);
		break;
	case kStop:
		DriverStation::ReportError("Auto Stop");
		m_counter = 20;
		m_rate = (m_maxspeed - m_minspeed) / m_counter;
		m_speed = m_speed - m_rate;
		m_stage = kShoot;
		m_drivetrain->Drive(0, m_speed);
		break;
	case kShoot:
		if (m_counter > 0)
		{
			if ((m_counter % 5) == 0)
				DriverStation::ReportError("Auto Shoot");
			m_counter--;
			m_speed = m_speed - m_rate;
			m_drivetrain->Drive(0, m_speed);
		}
		else
		{
			m_drivetrain->Drive(0, 0);
			m_stage = kIdle;
			m_vision->Loop(true, delay);
		}
		break;
	}
}


void Autonomous::Calibrate()
{
	m_gyro->Calibrate();
}
