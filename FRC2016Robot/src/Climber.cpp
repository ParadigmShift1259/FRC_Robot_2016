// climber.cpp


#include <Climber.h>
#include "Const.h"
#include <Spark.h>


Climber::Climber(OperatorInputs *operatorinputs)
{
	m_inputs = operatorinputs;
	m_motor = new Spark(PWM_CLIMBER_MOTOR);
	m_solenoid = new Solenoid(PCM_CLIMBER_SOLENOID);
	m_state = kDeploy;
	m_counter = 0;
}


Climber::~Climber()
{
	delete m_motor;
	delete m_solenoid;
}


void Climber::Init()
{
	m_motor->Set(0);
	m_solenoid->Set(false);
	m_state = kDeploy;
	m_counter = 0;
}


void Climber::Loop()
{
	bool deploybutton1 = m_inputs->joystickTrigger(OperatorInputs::ToggleChoice::kHold);
	bool deploybutton2 = m_inputs->button2(OperatorInputs::ToggleChoice::kHold);
	bool climbdownbutton = m_inputs->button3(OperatorInputs::ToggleChoice::kHold);
	bool climbupbutton = m_inputs->button5(OperatorInputs::ToggleChoice::kHold);

	switch (m_state)
	{
	case kDeploy:
		if (deploybutton1 && deploybutton2)		// deploy the climber
		{
			m_solenoid->Set(true);
			m_counter = 5;
			m_state = kDelay;
		}
		else
		if (climbupbutton || climbdownbutton)	// allow climbing independent of deploy
		{
			m_state = kClimb;
		}
		break;
	case kDelay:
		if (m_counter > 0)						// delay after deploy
		{
			m_counter--;
		}
		else
		{
			m_counter = 25;
			m_state = kClimb;
		}
		break;
	case kClimb:
		if (climbupbutton)						// raise the robot
		{
			if (m_counter > 0)
				m_counter--;
			else
				m_solenoid->Set(false);					// reset the solenoid
			m_motor->Set(-1);
		}
		else
		if (climbdownbutton)					// lower the robot
		{
			if (m_counter > 0)
				m_counter--;
			else
				m_solenoid->Set(false);					// reset the solenoid
			m_motor->Set(1);
		}
		else
		{
			m_motor->Set(0);						// turn off the winch
			m_state = kDeploy;						// go back to deploy state
		}
		break;
	}
}
