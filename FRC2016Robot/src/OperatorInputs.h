// operatorinputs.h


#ifndef SRC_OPERATORINPUTS_H_
#define SRC_OPERATORINPUTS_H_


#include <Joystick.h>


class OperatorInputs
{
public:
	OperatorInputs();
	~OperatorInputs();
	// xbox
	double xBoxLeftX();
    double xBoxRightX();
    double xBoxLeftY();
    double xBoxRightY();
	bool xBoxAButton();
    bool xBoxBButton();
    bool xBoxXButton();
	bool xBoxYButton();
    bool xBoxLeftBumper();
	bool xBoxRightBumper();
	bool xBoxLeftTrigger();
	bool xBoxRightTrigger();
	bool xBoxStartButton();
	bool xBoxBackButton();
	bool xBoxDPadUp();
	bool xBoxDPadDown();
	bool xBoxR3();
	// joystick
    double joystickX();
    double joystickY();
    double joystickZ();
    bool joystickTrigger();
	bool button7();
	bool button8();

private:
    double deadzoneFilterY(double joyStickValue);
    double deadzoneFilterX(double joyStickValue);
    double deadzoneFilterZ(double joyStickValue);

protected:
	 Joystick *m_joystick;
	 Joystick *m_xbox;
	 double m_triggerpressed;
};


#endif /* SRC_OPERATORINPUTS_H_ */
