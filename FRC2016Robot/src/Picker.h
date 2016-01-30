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
	void setDeployAction(bool deployaction);
	void StartMotor();
	bool motorDirection();  	//True would be like clockwise, and False etc.
	double motorSpeed();
	void movePicker();
	void pickUp();
	void shoot();

private:
	void pickerUp();
	void pickerDown();

protected:
	SpeedController* pickerMotor;
	Solenoid* pickerDeploy;
	Solenoid* pickerVent;
	OperatorInputs* xBox;
	bool deployed;
	int deployedcounter;
	bool previousA;
	bool previousB;
};


#endif /* SRC_PICKER_H_ */
