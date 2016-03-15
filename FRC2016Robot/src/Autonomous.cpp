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
	m_stage = kStop;
	m_driveangle = 0;
	m_counter = 0;
}


Autonomous::~Autonomous()
{
}


void Autonomous::Init()
{
	m_gyro->Reset();
	if (m_driverstation->IsAutonomous())
		m_stage = kDrive;
	else
		m_stage = kStop;
	m_driveangle = 0;
	m_counter = 0;
}


void Autonomous::Loop()
{
	double angle = m_gyro->GetAngle();
	SmartDashboard::PutNumber("Gyro Raw", angle);

	double driveangle = angle - m_driveangle;
	SmartDashboard::PutNumber("Gyro Heading", driveangle);

	double xaccel = m_accel->GetX();
	double yaccel = m_accel->GetY();
	double zaccel = m_accel->GetZ();
	SmartDashboard::PutNumber("Xaccel", xaccel);
	SmartDashboard::PutNumber("Yaccel", yaccel);
	SmartDashboard::PutNumber("Zaccel", zaccel);

	if ((!m_driverstation->IsAutonomous()) && (m_stage == kStop))
		return;

	double leftposition = m_drivetrain->LeftTalon()->GetPosition();
	double rightposition = m_drivetrain->RightTalon()->GetPosition();

	SmartDashboard::PutNumber("AUT1_leftpos", leftposition);
	SmartDashboard::PutNumber("AUT2_rightpos", rightposition);

	switch (m_stage)
	{
	case kStop:
		break;
	case kDrive:
		DriverStation::ReportError("kDrive\n");
		m_drivetrain->LeftTalon()->SetPosition(0);
		m_drivetrain->RightTalon()->SetPosition(0);
		m_drivetrain->Drive(0, -1);
		m_counter = 50;
		m_stage = kDriveLoop;
		break;
	case kDriveLoop:
		if (m_counter > 0)
		{
			m_counter--;
			if ((m_counter % 5) == 0)
				DriverStation::ReportError("kDriveLoop "+std::to_string(m_counter)+"\n");
			if ((abs(m_drivetrain->LeftTalon()->GetPosition()) > 18) &&
				(abs(m_drivetrain->RightTalon()->GetPosition()) > 18))
			{
				m_counter = 0;
				m_drivetrain->Drive(0, -0.25);
				m_stage = kShoot;
			}
			else
				m_drivetrain->Drive(0, -1);
		}
		else
			m_stage = kShoot;
		break;
	case kShoot:
		DriverStation::ReportError("kShoot\n");
		m_drivetrain->Drive(0, -0.25);
		m_counter = 10;
		m_stage = kShootLoop;
		break;
	case kShootLoop:
		if (m_counter > 0)
		{
			DriverStation::ReportError("kShootLoop\n");
			m_drivetrain->Drive(0, 0);
			m_counter--;
		}
		else
		{
			m_stage = kStop;
			m_vision->Loop(true);
		}
		break;
	}
}


void Autonomous::Calibrate()
{
	m_gyro->Calibrate();
}
