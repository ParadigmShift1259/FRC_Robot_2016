/*
 * OperatorInputs.h
 *
 *  Created on: Jan 15, 2016
 *      Author: Programming
 */

#ifndef SRC_OPERATORINPUTS_H_
#define SRC_OPERATORINPUTS_H_

#include <joystick.h>

class OperatorInputs
{
public:
	OperatorInputs();
	virtual ~OperatorInputs();
	bool xBoxStartButton();
	bool isShooterTriggerPressed();
	bool xBoxLeftTrigger();
	bool isSetKickerPositionButtonPressed();
	bool xBoxBackButton();
	bool button7();
	bool isPickerLoadingPositionButtonPressed();
	bool xBoxYButton();
	bool xBoxAButton();
    bool xBoxBButton();
    bool xBoxXButton();
    bool xBoxRightBumper();
    bool xBoxLeftBumper();
    bool isShootButtonPressed();
    double xboxRightX();
    double xboxRightY();
    double xboxLeftX();
    double xboxLeftY();
    double joystickX();
    double joystickY();
    double joystickZ();
    bool shifter();
    bool joystickTriggerPressed();
    bool joystickTriggerPressedAgain();

private:
    double deadzoneFilterY(double joyStickValue);
    double deadzoneFilterX(double joyStickValue);
    double deadzoneFilterZ(double joyStickValue);

protected:
	 double triggerPressed;

	 Joystick *joystick;
	 Joystick *xBox;

};

#endif /* SRC_OPERATORINPUTS_H_ */
