// picker.cpp


#include <Picker.h>
#include <Spark.h>
#include <Const.h>


Picker::Picker(OperatorInputs* operinputs)
{
	xBox = operinputs;
	pickerMotor = new Spark(INGEST_MOTOR);
	pickerDeploy = new Solenoid(PICKER_DEPLOY);
	pickerVent = new Solenoid(PICKER_VENT);
	deployed = false;
	deployedcounter = 0;
	previousA = false;
	previousB = false;
}


Picker::~Picker()
{
	delete pickerDeploy;
	delete pickerVent;
}


void Picker::StartMotor()
{
	pickerMotor->Set(1);
}


void Picker::setDeployAction(bool deployaction)
{
	deployed = deployaction;
}


bool Picker::motorDirection()
{
	return true;
}


double Picker::motorSpeed()
{
	return pickerMotor->Get();
}


void Picker::Loop()
{
	bool aButton = xBox->xBoxAButton();
	bool bButton = xBox->xBoxBButton();
	bool bumper = xBox->xBoxRightBumper();

	if (!(aButton && bButton))
	{
		if (aButton && !previousA)
		{
			setDeployAction(true);
			pickerDeploy->Set(true);
			deployedcounter = 0;
		}
		if (bButton && !previousB)
		{
			setDeployAction(false);
			pickerVent->Set(false);
			pickerDeploy->Set(deployed);
			deployedcounter = 0;
		}
	}

	if (deployed)
	{
		if (deployedcounter < PICKER_VENT_DELAY)
			deployedcounter++;
		else
			pickerVent->Set(true);
	}

	if (bumper)
	{
		pickerMotor->Set(-1);
	}
	else
	{
		pickerMotor->Set(1);
	}
	previousA = aButton;
	previousB = bButton;
}
