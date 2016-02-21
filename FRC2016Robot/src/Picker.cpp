// picker.cpp


#include <Picker.h>
#include <Const.h>
#include <Spark.h>


Picker::Picker(OperatorInputs* inputs)
{
	m_inputs = inputs;
	m_motor = new Spark(PWM_PICKER_MOTOR);
	m_solenoid = new Solenoid(PCM_PICKER_SOLENOID);
	m_state = kDown;
	m_counter = 0;
	m_prevreverse = false;
}


Picker::~Picker()
{
	delete m_motor;
	delete m_solenoid;
}


void Picker::Init()
{
	m_motor->Set(1);
	m_solenoid->Set(false);
	m_state = kDown;
	m_counter = 0;
	m_prevreverse = false;
}


void Picker::Loop()
{
	bool downbutton = m_inputs->xBoxAButton();
	bool upbutton = m_inputs->xBoxBButton();
	bool shootbutton = m_inputs->xBoxRightBumper();
	bool reversebutton = m_inputs->xBoxRightTrigger(OperatorInputs::ToggleChoice::kHold);

	if (reversebutton)
	{
		switch (m_state)
		{
		case kDown:
		case kShoot2:
		case kUpDelay:
		case kUp:
			m_motor->Set(-1);
			m_prevreverse = true;
			break;
		default:
			break;
		}
	}
	else
	if (m_prevreverse)
	{
		switch (m_state)
		{
		case kDown:
		case kShoot2:
			m_motor->Set(-1);
			break;
		case kUpDelay:
			m_motor->Set(1);
			break;
		case kUp:
			m_motor->Set(0);
			break;
		default:
			break;
		}
		m_prevreverse = false;
	}

	switch (m_state)
	{
	case kDown:
		if (shootbutton)
		{
			m_solenoid->Set(true);
			m_counter = 5;
			m_state = kShoot1;
		}
		else
		if (upbutton)
		{
			m_solenoid->Set(true);
			m_counter = 25;
			m_state = kUpDelay;
		}
		break;
	case kShoot1:
		if (m_counter > 0)
		{
			m_counter--;
		}
		else
		{
			m_motor->Set(-1);
			m_counter = 25;
			m_state = kShoot2;
		}
		break;
	case kShoot2:
		if (m_counter > 0)
		{
			m_counter--;
		}
		else
		{
			m_motor->Set(0);
			m_counter = 0;
			m_state = kUp;
		}
		break;
	case kUpDelay:
		if (m_counter > 0)
		{
			m_counter--;
		}
		else
		{
			m_motor->Set(0);
			m_counter = 0;
			m_state = kUp;
		}
		break;
	case kUp:
		if (downbutton)
		{
			m_solenoid->Set(false);
			m_motor->Set(1);
			m_state = kDown;
		}
		break;
	}
}
