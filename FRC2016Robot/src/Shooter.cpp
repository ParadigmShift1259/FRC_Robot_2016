// shooter.cpp


#include "Shooter.h"
#include "Spark.h"
#include "Const.h"


Shooter::Shooter(OperatorInputs* inputs)
{
	m_inputs = inputs;
	m_solenoid = new Solenoid(PCM_SHOOTER_SOLENOID);
	m_motor = new Spark(PWM_SHOOTER_MOTOR);
}


Shooter::~Shooter()
{
	delete m_solenoid;
	delete m_motor;
}


void Shooter::Loop()
{
	bool shootbutton = m_inputs->xBoxRightTrigger();

	if (shootbutton)
	{
		m_solenoid->Set(true);
	}
	else
	{
		m_solenoid->Set(false);
	}
}
