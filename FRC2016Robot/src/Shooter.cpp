// shooter.cpp


#include "Shooter.h"
#include "Spark.h"
#include "Const.h"


Shooter::Shooter(OperatorInputs* operinputs)
{
	inputs = operinputs;
	arm = new Solenoid(ACTUATOR_SOLENOID);
	wheelMotor = new Spark(PWM_SHOOTER);
}


Shooter::~Shooter()
{
	delete arm;
	delete wheelMotor;
}


void Shooter::Loop()
{
	if (inputs->xBoxRightTrigger())
	{
		arm->Set(true);
	}
	else
	{
		arm->Set(false);
	}
}
