// portcullis.cpp


#include <Portcullis.h>
#include <spark.h>
#include "Const.h"
#include <driverstation.h>


Portcullis::Portcullis(OperatorInputs *inputs)
{
	m_inputs = inputs;
	m_motor = new Spark(PWM_PORTCULLIS_MOTOR);
	m_limit = new DigitalInput(DIO_PORTCULLIS_LIMIT);
	m_state = kStored;
	m_counter = 0;
}


Portcullis::~Portcullis()
{
	delete m_motor;
	delete m_limit;
}


void Portcullis::Init()
{
	m_state = kStored;
	m_counter = 0;
}


void Portcullis::Loop()
{
	bool downbutton = m_inputs->xBoxXButton();
	bool upbutton = m_inputs->xBoxYButton();
	int maxcounter = 50;

	switch (m_state)
	{
	case kStored:					// portcullis bar is in the upright position
		if (downbutton)					// lower the bar
		{
			m_motor->Set(-1.0);				// reverse the motor
			m_counter = maxcounter;					// max runtime
			m_state = kLower;
		}
		if (!m_limit->Get() && upbutton)// bar didn't finish storing, try raising again
		{
			m_motor->Set(.75);				// turn on the motor
			m_counter = maxcounter;				// max runtime
			m_state = kRaise;
		}
		break;
	case kLower:					// portcullis bar is moving down
		if (m_counter > 0)
		{
			m_counter--;					// decrement max time counter
		}
		if ((m_counter <= (maxcounter-5)) &&
			m_limit->Get())				// limit switch tripped
		{
			m_counter = 0;					// stop moving bar down
		}
		if (m_counter <= 0)				// at deployed state
		{
			m_motor->Set(0);				// turn motor off
			m_counter = 0;
			m_state = kDeployed;
		}
		break;
	case kDeployed:					// portcullis bar is in the deployed position
		if (upbutton)					// raise the bar
		{
			m_motor->Set(.75);				// turn on the motor
			m_counter = maxcounter;					// max runtime
			m_state = kRaise;
		}
		if (!m_limit->Get() && downbutton)// bar didn't finish deploying, try deploying again
		{
			m_motor->Set(-1.0);				// turn on the motor
			m_counter = maxcounter;					// max runtime
			m_state = kRaise;
		}
		break;
	case kRaise:					// portcullis bar is moving up
		if (m_counter > 0)
		{
			m_counter--;					// decrement max time counter
		}
		if ((m_counter <= (maxcounter-5)) &&
			m_limit->Get())				// limit switch tripped
		{
			m_counter = 0;					// stop moving bar up
		}
		if (m_counter <= 0)				// at raised state
		{
			m_motor->Set(0);				// turn motor off
			m_counter = 0;
			m_state = kStored;
		}
		break;
	}
}
