// shooter.cpp


#include "Shooter.h"
#include "Const.h"
#include <Spark.h>
#include <smartdashboard/smartdashboard.h>
#include <driverstation.h>


Shooter::Shooter(OperatorInputs* inputs, Picker* picker)
{
	m_inputs = inputs;
	m_picker = picker;
	m_solenoid = new Solenoid(PCM_SHOOTER_SOLENOID);
	m_motor = new Spark(PWM_SHOOTER_MOTOR);
	m_limitdown = new DigitalInput(DIO_SHOOTER_LIMIT_DOWN);
	m_encoder = new Encoder(DIO_SHOOTER_MOTOR_A, DIO_SHOOTER_MOTOR_B);
	m_stage = kReady;
	m_counter = 0;
}


Shooter::~Shooter()
{
	delete m_solenoid;
	delete m_motor;
	delete m_limitdown;
	delete m_encoder;
}


void Shooter::Init()
{
	m_stage = kReady;
	m_counter = 0;
	m_motor->Set(0);
}


void Shooter::Loop(bool shoot)
{
	bool shootbutton = m_inputs->xBoxBackButton();
	bool winchdown = m_inputs->xBoxDPadRight(OperatorInputs::ToggleChoice::kHold);
	bool winchup = m_inputs->xBoxDPadLeft(OperatorInputs::ToggleChoice::kHold);

	// manual override
	if (winchdown)								// draw down the shooter
	{
		m_motor->Set(1);							// start winch
		int distance = m_encoder->Get();		// get distance the motor ran
		DriverStation::ReportError("Winch encoder: " + to_string(distance) + "\n");
		if (m_limitdown->Get())					// if limit switch pressed
			m_solenoid->Set(false);						// lock shooter
		m_stage = kOverride;
	}
	else
	if (winchup)								// reverse the winch
	{
		m_motor->Set(-1);							// reverse winch
		int distance = m_encoder->Get();		// get distance the motor ran
		DriverStation::ReportError("Winch encoder: " + to_string(distance) + "\n");
		m_stage = kOverride;
	}

	switch (m_stage)
	{
	case kReady:
	case kPickerDrop:
		if (shootbutton || shoot || (m_stage == kPickerDrop))				// shoot button pressed or shoot action
		{
			if (m_picker->GetState() != Picker::State::kDown)
			{
				m_picker->Loop(true);
				m_stage = kPickerDrop;
			}
			else
			{
				m_solenoid->Set(true);					// release shooter
				m_counter = 25;							// delay ~1 second before winching
				m_stage = kRelease;
			}
		}
		break;
	case kRelease:
		if (m_counter > 0)						// delay before winching
		{
			m_counter--;
		}
		if (m_counter <= 0)						// start winching
		{
			m_counter = 100;						// delay ~4 second max runtime before auto shutoff
			m_stage = kWinch;
		}
		break;
	case kWinch:
		if (m_counter > 0)						// still ok to run the winch
		{
			m_motor->Set(1);						// run the winch
			if (m_limitdown->Get())				// if limit switch pressed
				m_counter = 0;							// lock the shooter
			else
				m_counter--;							// still run the winch
		}
		if (m_counter <= 0)						// lock the shooter
		{
			m_solenoid->Set(false);					// engage the lock
			m_counter = 5;							// delay slightly while still winching
			m_stage = kLock;
		}
		break;
	case kLock:
		if (m_counter > 0)						// delay while winching
		{
			m_counter--;
		}
		if (m_counter <= 0)						// stop winching
		{
			m_motor->Set(0);						// turn off motor
			m_encoder->Reset();						// clear the encoder
			m_counter = 50;							// delay ~2 second max runtime before auto shutoff
			m_stage = kReverse;
		}
		break;
	case kReverse:
		if (m_counter > 0)						// continue reverse
		{
			m_motor->Set(-1);						// reverse motor
			m_counter--;
			int distance = m_encoder->Get();		// get distance the motor ran
			DriverStation::ReportError("Winch encoder: " + to_string(distance) + "\n");
			if (distance < -1400)
				m_counter = 0;						// motor should stop
		}
		if (m_counter <= 0)						// stop motor
		{
			m_motor->Set(0);						// stop motor
			m_encoder->Reset();						// clear the encoder
			m_counter = 0;							// reset the counter
			m_stage = kReady;
		}
		break;
	case kOverride:
		if (!winchdown && !winchup)				// no longer overriding
		{
			m_motor->Set(0);						// stop motor
			m_encoder->Reset();						// clear the encoder
			m_counter = 0;							// reset the counter
			m_stage = kReady;
		}
		break;
	}
}
