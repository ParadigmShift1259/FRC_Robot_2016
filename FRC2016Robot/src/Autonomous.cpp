// autonomous.cpp


#include "Autonomous.h"
#include "const.h"


Autonomous::Autonomous(OperatorInputs *operatorinputs, Drivetrain *drivetrain)
{
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_counter = 0;
}


Autonomous::~Autonomous()
{
}


void Autonomous::Init()
{
	m_counter = 0;
}


void Autonomous::Loop()
{
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
