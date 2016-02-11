// picker.h


#ifndef SRC_PICKER_H_
#define SRC_PICKER_H_


#include "SpeedController.h"
#include "Solenoid.h"
#include "OperatorInputs.h"


class Picker
{
public:
	Picker(OperatorInputs* operinputs);
	~Picker();
	void setDeployAction(bool deployaction);
	void StartMotor();
	bool motorDirection();  	//True would be like clockwise, and False etc.
	double motorSpeed();
	void Loop();
	void pickUp();
	void shoot();

private:
	void pickerUp();
	void pickerDown();

protected:
	OperatorInputs *xBox;
	SpeedController *pickerMotor;
	Solenoid *pickerDeploy;
	//Solenoid *pickerDeploy2;
	bool deployed;
	int deployedcounter;
	bool previousA;
	bool previousB;
};


#endif /* SRC_PICKER_H_ */
