// climber.cpp


#include <Climber.h>
#include <Spark.h>
#include "Const.h"


Climber::Climber(OperatorInputs *operatorinputs)
{
	m_inputs = operatorinputs;
	m_motor = new Spark(PWM_CLIMBER_MOTOR);
	m_solenoid = new Solenoid(PCM_CLIMBER_SOLENOID);
	m_deploytoggle = false;
}


Climber::~Climber()
{
	delete m_motor;
	delete m_solenoid;
}


void Climber::Loop()
{
	bool deploybutton = m_inputs->xBoxDPadUp();

	if (deploybutton && !m_deploytoggle)
	{
		m_solenoid->Set(true);
	}
	m_deploytoggle = deploybutton;

	if (m_inputs->xBoxDPadDown())
	{
		m_motor->Set(1);
	}
	else
	{
		m_motor->Set(0);
	}
}
