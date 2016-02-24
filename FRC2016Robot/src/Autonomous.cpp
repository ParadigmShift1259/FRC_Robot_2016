// autonomous.cpp


#include "Autonomous.h"
#include "const.h"
#include <driverstation.h>


Autonomous::Autonomous(DriverStation *driverstation, OperatorInputs *operatorinputs, Drivetrain *drivetrain)
{
	m_driverstation = driverstation;
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
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
	m_stage = kStop;
	m_driveangle = 0;
	m_counter = 0;
	while (!m_instructions.empty())
		m_instructions.pop();
	m_instructions.push({50, 0, 0.5});
	m_instructions.push({50, 90, 0.5});
	m_instructions.push({50, 90, 0.5});
	m_instructions.push({50, 90, 0.5});
	m_instructions.push({50, 90, 0.5});
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

	if (!m_driverstation->IsAutonomous())
		return;

	switch (m_stage)
	{
	case kStop:
		if (!m_instructions.empty())
		{
			m_instruction = m_instructions.front();
			m_instructions.pop();
			m_counter = m_instruction.time;
			m_driveangle += m_instruction.angle;
			m_stage = kDrive;
		}
		break;
	case kDrive:
		if (m_counter > 0)
		{
			m_drivetrain->Drive(driveangle * -0.03, m_instruction.speed);
			m_counter--;
		}
		else
		{
			m_stage = kStop;
		}
		break;
	}
}


void Autonomous::Calibrate()
{
	//m_gyro->Calibrate();
}
