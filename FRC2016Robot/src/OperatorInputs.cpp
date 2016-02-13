// operatorinputs.cpp


#include <OperatorInputs.h>
#include <cmath>
#include "Const.h"


using namespace std;


OperatorInputs::OperatorInputs()
{
	triggerPressed = 0;
	joystick = new Joystick(0);
	xBox = new Joystick(1);
}


OperatorInputs::~OperatorInputs()
{
	delete joystick;
	delete xBox;
}


bool OperatorInputs::xBoxDPadUp()
{
	return (xBox->GetPOV() == 0);
}


bool OperatorInputs::xBoxDPadDown()
{
	return (xBox->GetPOV() == 180);
}


bool OperatorInputs::xBoxStartButton()
{
	//Returns true if start button is pressed
	return xBox->GetRawButton(START_BUTTON);
}

bool OperatorInputs::xBoxR3()
{
	//Returns true if start button is pressed
	return xBox->GetRawButton(10);
}

bool OperatorInputs::xBoxRightTrigger()
{
	triggerPressed = xBox->GetRawAxis(XBOX_RIGHT_TRIGGER_AXIS);
	return (RIGHT_TRIGGER_MIN <= triggerPressed && triggerPressed <= RIGHT_TRIGGER_MAX);
}


bool OperatorInputs::xBoxLeftTrigger()
{
	triggerPressed = xBox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS);
	return (LEFT_TRIGGER_MIN <= triggerPressed && triggerPressed <= LEFT_TRIGGER_MAX);

}


bool OperatorInputs::isSetKickerPositionButtonPressed()
{
	triggerPressed = xBox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS);
	return (LEFT_TRIGGER_MIN <= triggerPressed && triggerPressed <= LEFT_TRIGGER_MAX);
}


bool OperatorInputs::xBoxBackButton()
{
	return xBox->GetRawButton(BACK_BUTTON);
}


bool OperatorInputs::button7()
{
	return joystick->GetRawButton(7);
}


bool OperatorInputs::button8()
{
	return joystick->GetRawButton(8);
}


bool OperatorInputs::isPickerLoadingPositionButtonPressed()
{
	return xBox->GetRawButton(A_BUTTON);
}


bool OperatorInputs::xBoxYButton()
{
	return xBox->GetRawButton(Y_BUTTON);
}


bool OperatorInputs::xBoxAButton()
{
	return xBox->GetRawButton(A_BUTTON);
}


bool OperatorInputs::xBoxBButton()
{
	return xBox->GetRawButton(B_BUTTON);
}


bool OperatorInputs::xBoxXButton()
{
	return xBox->GetRawButton(X_BUTTON);
}


bool OperatorInputs::xBoxRightBumper()
{
	return xBox->GetRawButton(RIGHT_BUMPER);
}


bool OperatorInputs::xBoxLeftBumper()
{
	return xBox->GetRawButton(LEFT_BUMPER);
}


bool OperatorInputs::isShootButtonPressed()
{
	return xBox->GetRawButton(LEFT_BUMPER);
}


double OperatorInputs::xboxRightX()
{
	return deadzoneFilterX(xBox->GetX(GenericHID::JoystickHand::kRightHand));
}


double OperatorInputs::xboxRightY()
{
	return deadzoneFilterY(xBox->GetY(GenericHID::JoystickHand::kRightHand));
}


double OperatorInputs::xboxLeftX()
{
	return deadzoneFilterX(INVERT_X_AXIS*xBox->GetX(GenericHID::JoystickHand::kLeftHand));
}


double OperatorInputs::xboxLeftY()
{
	return deadzoneFilterY(INVERT_Y_AXIS*xBox->GetY(GenericHID::JoystickHand::kLeftHand));
}


double OperatorInputs::joystickX()
{
	return deadzoneFilterX(INVERT_X_AXIS*joystick->GetX());
	//return joystick->GetX();
}


double OperatorInputs::joystickY()
{
	return deadzoneFilterY(INVERT_Y_AXIS*joystick->GetY());
	//return joystick->GetY();
}


double OperatorInputs::joystickZ()
{
	return deadzoneFilterZ(joystick->GetZ());
}


/**
 * returns a value 0 if the joystick value is within the dead zone (if the
 * joystick is outside of the dead zone it returns the joystick value)
 *
 * @param joyStickValue
 * @return
 */
double OperatorInputs::deadzoneFilterY(double joyStickValue)
{
	if (abs(joyStickValue) <= DEADZONE_Y)
	{
		return 0;
	}
	double sub = joyStickValue/abs(joyStickValue);
	return (joyStickValue-sub*DEADZONE_Y)/(1.0-DEADZONE_Y);
}


double OperatorInputs::deadzoneFilterX(double joyStickValue)
{
	if (abs(joyStickValue) <= DEADZONE_X)
	{
		return 0;
	}
	double sub = joyStickValue/abs(joyStickValue);
	return (joyStickValue-sub*DEADZONE_X)/(1.0-DEADZONE_X);
}


double OperatorInputs::deadzoneFilterZ(double joyStickValue)
{
	if (abs(joyStickValue) <= DEADZONE_Z)
	{
		return 0;
	}
	double sub = joyStickValue/abs(joyStickValue);
	return (joyStickValue-sub*DEADZONE_Z)/(1.0-DEADZONE_Z);
}


bool OperatorInputs::shifter()
{
	return joystick->GetTrigger();
}


bool OperatorInputs::joystickTriggerPressed()
{
	return joystick->GetTrigger();
}


bool OperatorInputs::joystickTriggerPressedAgain()
{
	return joystick->GetTrigger();
}
