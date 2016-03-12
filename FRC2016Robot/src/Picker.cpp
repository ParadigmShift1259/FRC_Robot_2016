// picker.cpp


#include <Picker.h>
#include <Const.h>
#include <Spark.h>


Picker::Picker(OperatorInputs* inputs, DriverStation* driverstation)
{
	m_inputs = inputs;
	m_driverstation = driverstation;
	m_motor = new Spark(PWM_PICKER_MOTOR);
	m_solenoid = new DoubleSolenoid(PCM_PICKER_SOLENOID_A, PCM_PICKER_SOLENOID_B);
	m_solenoid->Set(DoubleSolenoid::kReverse);
	m_vent = new Solenoid(PCM_PICKER_VENT);
	m_state = kUp;
	m_counter = 0;
	m_prevreverse = false;
	m_stop = 1;
}


Picker::~Picker()
{
	delete m_motor;
	delete m_solenoid;
	delete m_vent;
}


void Picker::Init()
{
	m_vent->Set(true);			// close vent
	m_counter = 0;
	m_prevreverse = false;
}


void Picker::Loop(bool drop, int nocamdelay)
{
	bool downbutton = m_inputs->xBoxAButton() || drop;
	bool upbutton = m_inputs->xBoxBButton();
	bool shootbutton = false; //m_inputs->xBoxRightBumper();
	bool reversebutton = m_inputs->xBoxRightTrigger(OperatorInputs::ToggleChoice::kHold) || m_inputs->xBoxRightBumper(OperatorInputs::ToggleChoice::kHold);
	bool motorbutton = m_inputs->xBoxStartButton();

	DriverStation::ReportError("Picker: " + std::to_string(nocamdelay));

	if (reversebutton)
	{
		switch (m_state)
		{
		case kDown:
		case kShoot2:
		case kUpDelay:
		case kUp:
			m_motor->Set(-1.0 * m_stop);
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
		case kShoot2:
			m_motor->Set(-1.0 * m_stop);
			break;
		case kDown:
		case kUpDelay:
			m_motor->Set(0.75 * m_stop);
			break;
		case kUp:
			m_motor->Set(0);
			break;
		default:
			break;
		}
		m_prevreverse = false;
	}
	if (motorbutton)
	{
		m_stop = 1 - m_stop;
		m_motor->Set(0.75 * m_stop);
	}

	switch (m_state)
	{
	case kDown:
		if (shootbutton)
		{
			m_vent->Set(true);			// close vent
			m_counter = 5 * nocamdelay;
			m_state = kShoot1;
		}
		else
		if (upbutton)
		{
			m_vent->Set(true);			// close vent
			m_counter = 5 * nocamdelay;
			m_state = kUpDelay;
		}
		else
		{
			m_vent->Set(false);			// open vent
		}
		break;
	case kShoot1:
		if (m_counter > 0)
		{
			m_counter--;
		}
		else
		{
			m_solenoid->Set(DoubleSolenoid::kReverse);
			m_counter = 5 * nocamdelay;
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
			m_motor->Set(-1.0 * m_stop);
			m_counter = 5 * nocamdelay;
			m_state = kUpDelay;
		}
		break;
	case kUpDelay:
		if (m_counter > 0)
		{
			m_counter--;
		}
		else
		{
			m_solenoid->Set(DoubleSolenoid::kReverse);
			m_motor->Set(0);
			m_counter = 0 * nocamdelay;
			m_state = kUp;
		}
		break;
	case kUp:
		if (downbutton)
		{
			m_solenoid->Set(DoubleSolenoid::kForward);
			m_motor->Set(0.75 * m_stop);
			m_state = kDownDelay;
			m_counter = 25 * nocamdelay;
		}
		break;
	case kDownDelay:
		if (m_counter > 0)
		{
			m_counter--;
		}
		else
		{
			m_counter = 0 * nocamdelay;
			m_stop = 1;
			m_state = kDown;
		}
		break;
	}
}
