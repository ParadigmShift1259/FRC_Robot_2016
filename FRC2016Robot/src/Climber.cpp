// climber.cpp


#include <Climber.h>
#include "Const.h"
#include <Spark.h>


Climber::Climber(OperatorInputs *operatorinputs)
{
	m_inputs = operatorinputs;
	m_motor = new Spark(PWM_CLIMBER_MOTOR);
	m_solenoid = new Solenoid(PCM_CLIMBER_SOLENOID);
}


Climber::~Climber()
{
	delete m_motor;
	delete m_solenoid;
}


void Climber::Loop()
{
	bool deploybutton = m_inputs->xBoxStartButton();
	bool climbdownbutton = m_inputs->xBoxDPadDown(OperatorInputs::ToggleChoice::kHold);
	bool climbupbutton = m_inputs->xBoxDPadDown(OperatorInputs::ToggleChoice::kHold);

	if (deploybutton)				// deploy the climber
	{
		m_solenoid->Set(true);
	}

	if (climbupbutton)				// raise the robot
	{
		m_motor->Set(1);
		m_solenoid->Set(false);
	}
	else
	if (climbdownbutton)			// lower the robot
	{
		m_motor->Set(-1);
	}
	else
		m_motor->Set(0);
}
