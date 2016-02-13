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
	bool xBoxDPadDown(ToggleChoice choice = kToggle);
	bool xBoxR3(ToggleChoice choice = kToggle);
	// joystick
    double joystickX();
    double joystickY();
    double joystickZ();
    bool joystickTrigger(ToggleChoice choice = kToggle);
	bool button7(ToggleChoice choice = kToggle);
	bool button8(ToggleChoice choice = kToggle);

private:
    bool toggle(string buttonName, bool buttonValue);
    double deadzoneFilterY(double joyStickValue);
    double deadzoneFilterX(double joyStickValue);
    double deadzoneFilterZ(double joyStickValue);

protected:
	 Joystick *m_joystick;
	 Joystick *m_xbox;
	 map<string, bool> m_togglebuttons;
};


#endif /* SRC_OPERATORINPUTS_H_ */
