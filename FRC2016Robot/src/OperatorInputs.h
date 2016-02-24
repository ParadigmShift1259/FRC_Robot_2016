// operatorinputs.h

#include <Joystick.h>
#include <map>


using namespace std;


#ifndef SRC_OPERATORINPUTS_H_
#define SRC_OPERATORINPUTS_H_


class OperatorInputs
{
public:
	enum ToggleChoice { kToggle = 0, kHold = 1 };

	OperatorInputs();
	~OperatorInputs();
	// xbox
	double xBoxLeftX();
	double xBoxRightX();
	double xBoxLeftY();
	double xBoxRightY();
	bool xBoxAButton(ToggleChoice choice = kToggle);
	bool xBoxBButton(ToggleChoice choice = kToggle);
	bool xBoxXButton(ToggleChoice choice = kToggle);
	bool xBoxYButton(ToggleChoice choice = kToggle);
	bool xBoxLeftBumper(ToggleChoice choice = kToggle);
	bool xBoxRightBumper(ToggleChoice choice = kToggle);
	bool xBoxLeftTrigger(ToggleChoice choice = kToggle);
	bool xBoxRightTrigger(ToggleChoice choice = kToggle);
	bool xBoxStartButton(ToggleChoice choice = kToggle);
	bool xBoxBackButton(ToggleChoice choice = kToggle);
	bool xBoxDPadUp(ToggleChoice choice = kToggle);
	bool xBoxDPadRight(ToggleChoice choice = kToggle);
	bool xBoxDPadDown(ToggleChoice choice = kToggle);
	bool xBoxDPadLeft(ToggleChoice choice = kToggle);
	bool xBoxR3(ToggleChoice choice = kToggle);
	// joystick
	double joystickX();
	double joystickY();
	double joystickZ();
	bool joystickTrigger(ToggleChoice choice = kToggle);
	bool joystickAxis0Left(ToggleChoice choice = kToggle);
	bool joystickAxis0Right(ToggleChoice choice = kToggle);
	bool joystickAxis1Back(ToggleChoice choice = kToggle);
	bool joystickAxis1Forward(ToggleChoice choice = kToggle);
	bool button2(ToggleChoice choice = kToggle);
	bool button3(ToggleChoice choice = kToggle);
	bool button5(ToggleChoice choice = kToggle);
	bool button6(ToggleChoice choice = kToggle);
	bool button7(ToggleChoice choice = kToggle);
	bool button8(ToggleChoice choice = kToggle);
	bool button9(ToggleChoice choice = kToggle);
	bool button10(ToggleChoice choice = kToggle);

	Joystick *m_joystick;
	Joystick *m_xbox;

private:
	bool toggle(string buttonName, bool buttonValue);
	double deadzoneFilterY(double joyStickValue);
	double deadzoneFilterX(double joyStickValue);
	double deadzoneFilterZ(double joyStickValue);

protected:
	 map<string, bool> m_togglebuttons;
};


#endif /* SRC_OPERATORINPUTS_H_ */
