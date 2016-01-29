/*
 * Picker.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: Developer
 */

#include <Picker.h>
#include <Spark.h>
#include <Const.h>

Picker::Picker(OperatorInputs* inputs)
{
	pickerMotor = new Spark(INGEST_MOTOR);
	pickerSolenoid = new Solenoid(PICKER_SOLENOID);
	xBox = inputs;
	solenoidAction = false;
	previousA = false;
	previousB = false;
}

Picker::~Picker()
{
	delete pickerSolenoid;
	delete xBox;
}

void Picker::StartMotor()
{
	pickerMotor->Set(1);
}

void Picker::setSolenoidAction(bool sa)
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
	bool aButton = xBox->xBoxAButton();
	bool bButton = xBox->xBoxBButton();
	bool bumper = xBox->xBoxRightBumper();
	if(!(aButton&&bButton))
	{
		if(aButton&&!previousA)
		{
			setSolenoidAction(true);
			pickerSolenoid->Set(solenoidAction);
		}

		if(bButton&&!previousB)
		{
			setSolenoidAction(false);
			pickerSolenoid->Set(solenoidAction);
		}

		if(bumper)
		{
			pickerMotor->Set(-1);
		}
		else
		{
			pickerMotor->Set(1);
		}

	}
	previousA = aButton;
	previousB = bButton;
}
