/*
 * OperatorInputs.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: Programming
 */

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



    //static variables can benefit compile size
//    public OperatorInputs() {
//        this.shiftUp = true;
//    }


bool OperatorInputs::xBoxStartButton()
{ //Returns true if start button is pressed
	return xBox->GetRawButton(START_BUTTON);
}

bool OperatorInputs::isShooterTriggerPressed() {
        triggerPressed = xBox->GetRawAxis(XBOX_RIGHT_TRIGGER_AXIS);
        return (RIGHT_TRIGGER_MIN <= triggerPressed && triggerPressed <= RIGHT_TRIGGER_MAX);
    }

bool OperatorInputs::isLeftShooterTriggerPressed() {
        triggerPressed = xBox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS);
        return (LEFT_TRIGGER_MIN <= triggerPressed && triggerPressed <= LEFT_TRIGGER_MAX);

    }

bool OperatorInputs::isSetKickerPositionButtonPressed() {
        triggerPressed = xBox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS);
        return (LEFT_TRIGGER_MIN <= triggerPressed && triggerPressed <= LEFT_TRIGGER_MAX);
    }

bool OperatorInputs::xBoxBackButton() {
        return xBox->GetRawButton(BACK_BUTTON);
    }

bool OperatorInputs::button7() {
        return joystick->GetRawButton(7);
    }

bool OperatorInputs::isPickerLoadingPositionButtonPressed() {
        return xBox->GetRawButton(A_BUTTON);
    }

bool OperatorInputs::xBoxYButton() {
        return xBox->GetRawButton(Y_BUTTON);
    }

bool OperatorInputs::xBoxAButton() {
        return xBox->GetRawButton(A_BUTTON);
    }

bool OperatorInputs::xBoxBButton() {
        return xBox->GetRawButton(B_BUTTON);
    }

bool OperatorInputs::xBoxXButton() {
        return xBox->GetRawButton(X_BUTTON);
    }

bool OperatorInputs::xBoxRightBumper() {
        return xBox->GetRawButton(RIGHT_BUMPER);
    }

bool OperatorInputs::xBoxLeftBumper() {
        return xBox->GetRawButton(LEFT_BUMPER);
    }

bool OperatorInputs::isShootButtonPressed() {
        return xBox->GetRawButton(LEFT_BUMPER);
    }
//Commented out because I don't feel like figuring out the xbox controller stuff
/**
double OperatorInputs::xboxRightX() {
        return deadzoneFilterX(xBox->GetX(joystick->Hand.kRight));
    }

double OperatorInputs::xboxRightY() {
        return deadzoneFilterY(xBox->GetY(joystick->Hand.kRight));
    }

double OperatorInputs::xboxLeftX() {
        return deadzoneFilterX(xBox->GetX(joystick->Hand.kLeft));
    }

double OperatorInputs::xboxLeftY() {
        return deadzoneFilterY(xBox->GetY(joystick->hand.kLeft));
    }
*/
double OperatorInputs::joystickX() {
        return deadzoneFilterX(joystick->GetX());
		//return joystick->GetX();
    }

double OperatorInputs::joystickY() {
		return deadzoneFilterY(joystick->GetY());
		//return joystick->GetY();
    }

double OperatorInputs::joystickZ() {
        return deadzoneFilterX(joystick->GetZ());
    }

    /**
     * returns a value 0 if the joystick value is within the dead zone (if the
     * joystick is outside of the dead zone it returns the joystick value)
     *
     * @param joyStickValue
     * @return
     */
double OperatorInputs::deadzoneFilterY(double joyStickValue) {
        if (abs(joyStickValue) <= DEADZONE_Y) {
            return 0;
        }
        double sub = joyStickValue/abs(joyStickValue);
        return (joyStickValue-sub*DEADZONE_Y)/(1.0-DEADZONE_Y);

    }

double OperatorInputs::deadzoneFilterX(double joyStickValue) {
        if (abs(joyStickValue) <= DEADZONE_X) {
            return 0;
        }
        double sub = joyStickValue/abs(joyStickValue);
        return (joyStickValue-sub*DEADZONE_X)/(1.0-DEADZONE_X);
    }

bool OperatorInputs::shifter() {
        return joystick->GetTrigger();

    }

bool OperatorInputs::joystickTriggerPressed() {
        return joystick->GetTrigger();

    }

bool OperatorInputs::joystickTriggerPressedAgain() {
        return joystick->GetTrigger();

    }
