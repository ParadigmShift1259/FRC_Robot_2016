// picker.cpp


#include <Picker.h>
#include <Const.h>
#include <Spark.h>


Picker::Picker(OperatorInputs* inputs)
{
	m_inputs = inputs;
	m_motor = new Spark(PWM_PICKER_MOTOR);
	m_solenoid = new Solenoid(PCM_PICKER_SOLENOID);
	m_down = false;
	m_prevdown = false;
	m_prevup = false;
}


Picker::~Picker()
{
	delete m_motor;
	delete m_solenoid;
}


void Picker::Init()
{
	m_motor->Set(1);
	m_solenoid->Set(m_down);
}


void Picker::Loop()
{
	bool downbutton = m_inputs->xBoxAButton();
	bool upbutton = m_inputs->xBoxBButton();
	bool motorbutton = m_inputs->xBoxRightBumper();

	if (!(downbutton && upbutton))
	{
		if (downbutton && !m_prevdown)
			m_down = true;
		if (upbutton && !m_prevup)
			m_down = false;
		m_solenoid->Set(m_down);
	}
	m_prevdown = downbutton;
	m_prevup = upbutton;

	if (motorbutton)
	{
		m_motor->Set(-1);
	}
	else
	if (m_down)
	{
		m_motor->Set(1);
	}
	else
	if (!m_down && !motorbutton)
	{
		m_motor->Set(0);
	}
}
