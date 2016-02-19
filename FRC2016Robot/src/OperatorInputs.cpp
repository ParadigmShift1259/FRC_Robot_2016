// operatorinputs.cpp


#include <OperatorInputs.h>
#include "Const.h"
#include <cmath>


using namespace std;


OperatorInputs::OperatorInputs()
{
	m_joystick = new Joystick(0);
	m_xbox = new Joystick(1);
	m_joystick2 = new Joystick(2);
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


bool OperatorInputs::xBoxAButton(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(A_BUTTON);

	if (choice == kToggle)
		return toggle("xBoxAButton", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxBButton(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(B_BUTTON);

	if (choice == kToggle)
		return toggle("xBoxBButton", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxXButton(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(X_BUTTON);

	if (choice == kToggle)
		return toggle("xBoxXButton", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxYButton(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(Y_BUTTON);

	if (choice == kToggle)
		return toggle("xBoxYButton", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxLeftBumper(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(LEFT_BUMPER);

	if (choice == kToggle)
		return toggle("xBoxLeftBumper", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxRightBumper(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(RIGHT_BUMPER);

	if (choice == kToggle)
		return toggle("xBoxRightBumper", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxLeftTrigger(ToggleChoice choice)
{
	double axis = m_xbox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS);

	if (choice == kToggle)
		return toggle("xBoxLeftTrigger", (LEFT_TRIGGER_MIN <= axis) && (axis <= LEFT_TRIGGER_MAX));
	if (choice == kHold)
		return ((LEFT_TRIGGER_MIN <= axis) && (axis <= LEFT_TRIGGER_MAX));
	return false;
}


bool OperatorInputs::xBoxRightTrigger(ToggleChoice choice)
{
	double axis = m_xbox->GetRawAxis(XBOX_RIGHT_TRIGGER_AXIS);

	if (choice == kToggle)
		return toggle("xBoxRightTrigger", (RIGHT_TRIGGER_MIN <= axis && axis <= RIGHT_TRIGGER_MAX));
	if (choice == kHold)
		return (RIGHT_TRIGGER_MIN <= axis && axis <= RIGHT_TRIGGER_MAX);
	return false;
}


bool OperatorInputs::xBoxStartButton(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(START_BUTTON);

	if (choice == kToggle)
		return toggle("xBoxStartButton", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxBackButton(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(BACK_BUTTON);

	if (choice == kToggle)
		return toggle("xBoxBackButton", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxDPadUp(ToggleChoice choice)
{
	bool button = (m_xbox->GetPOV() == 0);

	if (choice == kToggle)
		return toggle("xBoxDPadUp", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxDPadDown(ToggleChoice choice)
{
	bool button = (m_xbox->GetPOV() == 180);

	if (choice == kToggle)
		return toggle("xBoxDPadDown", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::xBoxR3(ToggleChoice choice)
{
	bool button = m_xbox->GetRawButton(10);

	if (choice == kToggle)
		return toggle("xBoxR3", button);
	if (choice == kHold)
		return button;
	return false;
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


bool OperatorInputs::joystickTrigger(ToggleChoice choice)
{
	bool button = m_joystick->GetTrigger();

	if (choice == kToggle)
		return toggle("joystickTrigger", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::button7(ToggleChoice choice)
{
	bool button = m_joystick->GetRawButton(7);

	if (choice == kToggle)
		return toggle("joystickbutton7", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::button8(ToggleChoice choice)
{
	bool button = m_joystick->GetRawButton(8);

	if (choice == kToggle)
		return toggle("joystickbutton8", button);
	if (choice == kHold)
		return button;
	return false;
}


bool OperatorInputs::toggle(string buttonname, bool buttonval)
{
	bool toggleval = !m_togglebuttons[buttonname] && buttonval;
	m_togglebuttons[buttonname] = buttonval;
	return toggleval;
}


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
