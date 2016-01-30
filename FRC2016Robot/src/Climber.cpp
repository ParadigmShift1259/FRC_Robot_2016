// climber.cpp


#include <Climber.h>
#include <Spark.h>
#include "Const.h"


Climber::Climber(OperatorInputs *inputs)
{
	Spark1 = new Spark(SPARK_WINCH);
	PistonActivator = new Solenoid(PISTON);
	Input = inputs;
}


Climber::~Climber()
{
	delete Spark1;
	delete PistonActivator;
}


void Climber::WinchStuff()
{
	if (Input->xBoxDPadUp())
	{
		PistonActivator->Set(true);
	}
	else
	if (Input->xBoxDPadDown())
	{
		Spark1->Set(1);
	}
	else
	{
		Spark1->Set(0);
	}
}
