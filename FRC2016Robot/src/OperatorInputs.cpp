// operatorinputs.cpp


#include <OperatorInputs.h>
#include "Const.h"
#include <cmath>


using namespace std;


OperatorInputs::OperatorInputs()
{
	m_joystick = new Joystick(0);
	m_xbox = new Joystick(1);
	m_triggerpressed = 0;
}


OperatorInputs::~OperatorInputs()
{
	delete m_joystick;
	delete m_xbox;
}


double OperatorInputs::xBoxLeftX()
{
	return deadzoneFilterX(INVERT_X_AXIS * m_xbox->GetX(GenericHID::JoystickHand::kLeftHand));
}


double OperatorInputs::xBoxRightX()
{
	return deadzoneFilterX(m_xbox->GetX(GenericHID::JoystickHand::kRightHand));
}


double OperatorInputs::xBoxLeftY()
{
	return deadzoneFilterY(INVERT_Y_AXIS * m_xbox->GetY(GenericHID::JoystickHand::kLeftHand));
}


double OperatorInputs::xBoxRightY()
{
	return deadzoneFilterY(m_xbox->GetY(GenericHID::JoystickHand::kRightHand));
}


bool OperatorInputs::xBoxAButton()
{
	return m_xbox->GetRawButton(A_BUTTON);
}


bool OperatorInputs::xBoxBButton()
{
	return m_xbox->GetRawButton(B_BUTTON);
}


bool OperatorInputs::xBoxXButton()
{
	return m_xbox->GetRawButton(X_BUTTON);
}


bool OperatorInputs::xBoxYButton()
{
	return m_xbox->GetRawButton(Y_BUTTON);
}


bool OperatorInputs::xBoxLeftBumper()
{
	return m_xbox->GetRawButton(LEFT_BUMPER);
}


bool OperatorInputs::xBoxRightBumper()
{
	return m_xbox->GetRawButton(RIGHT_BUMPER);
}


bool OperatorInputs::xBoxLeftTrigger()
{
	m_triggerpressed = m_xbox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS);
	return (LEFT_TRIGGER_MIN <= m_triggerpressed && m_triggerpressed <= LEFT_TRIGGER_MAX);

}


bool OperatorInputs::xBoxRightTrigger()
{
	m_triggerpressed = m_xbox->GetRawAxis(XBOX_RIGHT_TRIGGER_AXIS);
	return (RIGHT_TRIGGER_MIN <= m_triggerpressed && m_triggerpressed <= RIGHT_TRIGGER_MAX);
}


bool OperatorInputs::xBoxStartButton()
{
	//Returns true if start button is pressed
	return m_xbox->GetRawButton(START_BUTTON);
}


bool OperatorInputs::xBoxBackButton()
{
	return m_xbox->GetRawButton(BACK_BUTTON);
}


bool OperatorInputs::xBoxDPadUp()
{
	return (m_xbox->GetPOV() == 0);
}


bool OperatorInputs::xBoxDPadDown()
{
	return (m_xbox->GetPOV() == 180);
}


bool OperatorInputs::xBoxR3()
{
	return m_xbox->GetRawButton(10);
}


double OperatorInputs::joystickX()
{
	return deadzoneFilterX(INVERT_X_AXIS * m_joystick->GetX());
}


double OperatorInputs::joystickY()
{
	return deadzoneFilterY(INVERT_Y_AXIS * m_joystick->GetY());
}


double OperatorInputs::joystickZ()
{
	return deadzoneFilterZ(m_joystick->GetZ());
}


bool OperatorInputs::joystickTrigger()
{
	return m_joystick->GetTrigger();
}


bool OperatorInputs::button7()
{
	return m_joystick->GetRawButton(7);
}


bool OperatorInputs::button8()
{
	return m_joystick->GetRawButton(8);
}


/**
 * returns a value 0 if the joystick value is within the dead zone (if the
 * joystick is outside of the dead zone it returns the joystick value)
 *
 * @param joyStickValue
 * @return
 */
double OperatorInputs::deadzoneFilterX(double joyStickValue)
{
	if (abs(joyStickValue) <= DEADZONE_X)
	{
		return 0;
	}
	double sub = joyStickValue / abs(joyStickValue);
	return (joyStickValue - (sub * DEADZONE_X)) / (1.0 - DEADZONE_X);
}


double OperatorInputs::deadzoneFilterY(double joyStickValue)
{
	if (abs(joyStickValue) <= DEADZONE_Y)
	{
		return 0;
	}
	double sub = joyStickValue / abs(joyStickValue);
	return (joyStickValue - (sub * DEADZONE_Y)) / (1.0 - DEADZONE_Y);
}


double OperatorInputs::deadzoneFilterZ(double joyStickValue)
{
	if (abs(joyStickValue) <= DEADZONE_Z)
	{
		return 0;
	}
	double sub = joyStickValue / abs(joyStickValue);
	return (joyStickValue - (sub * DEADZONE_Z)) / (1.0-DEADZONE_Z);
}
