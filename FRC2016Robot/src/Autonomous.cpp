// autonomous.cpp


#include "Autonomous.h"
#include "const.h"
#include <driverstation.h>


Autonomous::Autonomous(OperatorInputs *operatorinputs, Drivetrain *drivetrain)
{
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_gyro = new AnalogGyro(ALG_AUTONOMOUS_GYRO);
	m_accel = new BuiltInAccelerometer(Accelerometer::Range::kRange_4G);
	m_angle = 0;
	m_counter = 0;
}


Autonomous::~Autonomous()
{
}


void Autonomous::Init()
{
	m_gyro->Reset();
	m_gyro->Calibrate();
	m_counter = 0;
}


void Autonomous::Loop()
{
	m_angle = m_gyro->GetAngle();
	SmartDashboard::PutNumber("Gyro", m_angle);

	double xaccel = m_accel->GetX();
	double yaccel = m_accel->GetY();
	double zaccel = m_accel->GetZ();
	SmartDashboard::PutNumber("Xaccel", xaccel);
	SmartDashboard::PutNumber("Yaccel", yaccel);
	SmartDashboard::PutNumber("Zaccel", zaccel);

/*
	if (m_counter < 4)
	{
		if (m_drivetrain->getIsDoneDriving() == false)
		{
			m_drivetrain->driveDistance(2.0);
		}
		if (m_drivetrain->getIsDoneDriving() == true)
		{
			m_drivetrain->setAngle(90);
		}
		if (m_drivetrain->getIsTurning() == true && m_drivetrain->getIsDoneDriving() == true)
		{
			//m_drivetrain->turnAngle();
		}
		if (m_drivetrain->getIsTurning() == false && m_drivetrain->getIsDoneDriving() == true)
		{
			m_counter++;
			m_drivetrain->driveDistance(2);
		}
	}
*/
}
