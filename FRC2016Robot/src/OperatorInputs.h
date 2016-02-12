// operatorinputs.h


#ifndef SRC_OPERATORINPUTS_H_
#define SRC_OPERATORINPUTS_H_


#include <Joystick.h>


class OperatorInputs
{
public:
	OperatorInputs();
	virtual ~OperatorInputs();
	bool xBoxStartButton();
	bool xBoxDPadUp();
	bool xBoxDPadDown();
	bool xBoxRightTrigger();
	bool xBoxLeftTrigger();
	bool xBoxR3();
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
	 Joystick *joystick;
	 Joystick *xBox;
	 double triggerPressed;
};


#endif /* SRC_OPERATORINPUTS_H_ */
