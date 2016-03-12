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
	m_kS = SmartDashboard::GetNumber("DB/Slider 2", 0);
	if (m_kS == 0) {m_kS = 0.0025;}
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
	m_instructions.push({140, 0, 6000, 0, 0.0003, kDrive});
	m_instructions.push({0,  0,   0,   0,     0, kShoot});
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

	if ((!m_driverstation->IsAutonomous()) && (m_stage==kStop))
		return;
	else
	if (!m_driverstation->IsAutonomous())
	{
		m_counter = 0;
		while (!m_instructions.empty())
			m_instructions.pop();
	}

	double leftposition = m_drivetrain->LeftTalon()->GetPosition();
	double rightposition = m_drivetrain->RightTalon()->GetPosition();

	SmartDashboard::PutNumber("AUT1_leftpos", leftposition);
	SmartDashboard::PutNumber("AUT2_rightpos", rightposition);

	switch (m_stage)
	{
	case kStop:
		if (!m_instructions.empty())
		{
			m_instruction = m_instructions.front();
			m_instructions.pop();
			m_counter = m_instruction.time;
			m_driveangle += m_instruction.angle;
			m_stage = m_instruction.stage;
			DriverStation::ReportError("kStop\n");
		}
		break;
	case kDrive:
		if (m_counter > 0)
		{
			//m_drivetrain->Shift();
			m_drivetrain->EnablePID(m_instruction.kP,m_instruction.kI,0,0,-(m_instruction.distance*m_kS),(m_instruction.distance*m_kS));
			m_counter--;
			m_stage = kDriveLoop;
			DriverStation::ReportError("kDrive\n");
			//DriverStation::ReportError("kDriveLoop "+std::to_string(m_counter));
		}
		else
		{
			m_drivetrain->DisablePID();
			m_stage = kStop;
		}
		break;
	case kDriveLoop:
		if (m_counter > 0)
		{
			m_counter--;
			DriverStation::ReportError("kDriveLoop "+std::to_string(m_counter)+"\n");
		}
		else
		{
			DriverStation::ReportError("\n");
			//m_drivetrain->Shift();
			m_drivetrain->DisablePID();
			m_stage = kStop;
		}
		break;
	case kShoot:
		DriverStation::ReportError("kShoot\n");
		m_stage = kStop;
		break;
	}
}


void Autonomous::Calibrate()
{
	m_gyro->Calibrate();
}
