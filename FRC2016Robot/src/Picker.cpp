// picker.cpp


#include <Picker.h>
#include <Spark.h>
#include <Const.h>


Picker::Picker(OperatorInputs* operinputs)
{
	xBox = operinputs;
	pickerMotor = new Spark(INGEST_MOTOR);
	pickerDeploy = new Solenoid(PICKER_DEPLOY);
	//pickerDeploy2 = new Solenoid(PICKER_VENT);
	//pickerDeploy2->Set(false);
	deployed = false;
	pickerDeploy->Set(deployed);
	deployedcounter = 0;
	previousA = false;
	previousB = false;
}


Picker::~Picker()
{
	delete pickerDeploy;
	//delete pickerDeploy2;
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
	//StartMotor();
	bool aButton = xBox->xBoxAButton();
	bool bButton = xBox->xBoxBButton();
	bool bumper = xBox->xBoxRightBumper();

	if (!(aButton && bButton))
	{
		if (aButton && !previousA)
		{
			setDeployAction(true);
		}
		if (bButton && !previousB)
			setDeployAction(false);
		pickerDeploy->Set(deployed);
		//pickerDeploy2->Set(!deployed);
		deployedcounter = 0;
	}
/*
	if (deployed)
	{
		if (deployedcounter < PICKER_VENT_DELAY)
			deployedcounter++;
		else
			pickerDeploy2->Set(true);
	}*/

	if (bumper)
	{
		pickerMotor->Set(-1);
	}
	else if(!deployed)
	{
		pickerMotor->Set(1);
	}
	else
	{
		pickerMotor->Set(0);
	}
	previousA = aButton;
	previousB = bButton;
}
