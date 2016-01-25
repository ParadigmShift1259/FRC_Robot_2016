/*
 * Picker.h
 *
 *  Created on: Jan 23, 2016
 *      Author: Developer
 */

#ifndef SRC_PICKER_H_
#define SRC_PICKER_H_
#include "SpeedController.h"
#include "Solenoid.h"
#include "OperatorInputs.h"

class Picker
{

public:
	Picker(OperatorInputs* inputs);
	~Picker();
	bool solenoidAction;
	bool setSolenoidAction(bool sa);
	bool motorDirection();  //True would be like clockwise, and False etc.
	double motorSpeed();
	void movePicker();
	void pickUp();
	void shoot();

private:
	void pickerUp();
	void pickerDown();

protected:
	SpeedController* pickerMotor;
	Solenoid* pickerSolenoid;
	OperatorInputs* xBox;
	bool previousLeftBumper;
	bool previousLeftTrigger;

};

#endif /* SRC_PICKER_H_ */
