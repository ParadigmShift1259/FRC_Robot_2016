// shooter.cpp


#include "Shooter.h"
#include "Spark.h"
#include "Const.h"


Shooter::Shooter(OperatorInputs* inputs)
{
	m_inputs = inputs;
	m_solenoid = new Solenoid(PCM_SHOOTER_SOLENOID);
	m_motor = new Spark(PWM_SHOOTER_MOTOR);
	m_limitdown = new DigitalInput(DIO_SHOOTER_LIMIT_DOWN);
	m_stage = kReady;
	m_counter = 0;
}


Shooter::~Shooter()
{
	delete m_solenoid;
	delete m_motor;
	delete m_limitdown;
}


void Shooter::Loop(bool shoot)
{
	bool shootbutton = m_inputs->xBoxRightTrigger();

	switch (m_stage)
	{
	case kReady:
		if (shootbutton || shoot)
		{
			m_solenoid->Set(true);
			m_stage = kRelease;
			m_counter = 0;
		}
		break;
	case kRelease:
		if (m_counter < 50)
		{
			m_solenoid->Set(false);
			m_counter++;
		}
		if (m_counter > 50)
		{
			m_stage = kWinch;
			m_counter = 0;
		}
		break;
	case kWinch:
		if (m_limitdown->Get())
		{
			m_motor->Set(1);
			m_counter++;
		}
		if (!m_limitdown->Get())
		{
			m_motor->Set(0);
			m_stage = kReverse;
		}
		break;
	case kReverse:
		if (m_counter > 0)
		{
			m_motor->Set(-1);
			m_counter--;
		}
		if (m_counter <= 0)
		{
			m_motor->Set(0);
			m_counter = 0;
			m_stage = kReady;
		}
	}
}
