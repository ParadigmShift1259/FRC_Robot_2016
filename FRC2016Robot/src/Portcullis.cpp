// portcullis.cpp


#include <Portcullis.h>
#include <spark.h>
#include "Const.h"


Portcullis::Portcullis(OperatorInputs *inputs)
{
	m_inputs = inputs;
	m_motor = new Spark(PWM_PORTCULLIS_MOTOR);
	m_limitdown = new DigitalInput(DIO_PORTCULLIS_LIMIT_DOWN);
	m_limitup = new DigitalInput(DIO_PORTCULLIS_LIMIT_UP);
}


Portcullis::~Portcullis()
{
	delete m_motor;
	delete m_limitdown;
	delete m_limitup;
}


void Portcullis::Loop()
{
	bool downbutton = m_inputs->xBoxXButton();
	bool upbutton = m_inputs->xBoxYButton();

	if (downbutton)
	{
		m_motor->Set(1);
	}

	if (upbutton)
	{
		m_motor->Set(-1);
	}

	if (!m_limitdown->Get())
	{
		m_motor->Set(0);
	}

	if (!m_limitup->Get())
	{
		m_motor->Set(0);
	}
}
