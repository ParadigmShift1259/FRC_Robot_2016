/*
 * Picker.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: Developer
 */

#include <Picker.h>

Picker::Picker(OperatorInputs* inputs)
{
	pickerMotor = 0;
	pickerSolenoid = new Solenoid(3);
	xBox = inputs;
	solenoidAction = false;
	previousLeftBumper = false;
	previousLeftTrigger = false;
}

Picker::~Picker()
{
	delete pickerSolenoid;
	delete xBox;
}

bool Picker::setSolenoidAction(bool sa)
{
	solenoidAction = sa;
}

bool Picker::motorDirection()
{
	return true;
}

double Picker::motorSpeed()
{
	return pickerMotor->Get();
}


void Picker::movePicker()
{
	bool leftBumper = xBox->xBoxLeftBumper();
	bool leftTrigger = xBox->xBoxLeftTrigger();
	if(!(leftBumper&&leftTrigger))
	{
		if(leftBumper&&!previousLeftBumper)
		{
			setSolenoidAction(true);
			pickerSolenoid->Set(solenoidAction);

		}

		if(leftTrigger&&!previousLeftTrigger)
		{
			setSolenoidAction(false);
			pickerSolenoid->Set(solenoidAction);
		}
	}
}
