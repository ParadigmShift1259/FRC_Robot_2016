#include <Climber.h>
#include <Spark.h>

Climber::Climber(OperatorInputs *inputs)
{


	Spark1 = new Spark(0);
	PistonActivator = new Solenoid(0,1);
	Input = inputs;

}

Climber::~Climber()
{
	delete Spark1;
	delete PistonActivator;
}

void Climber::WinchStuff()
{
	if(Input->xBoxDPadUp())
	{
		PistonActivator->Set(true);
	}
	if(Input->xBoxDPadDown)
	{
		Spark1->Set(1);
	}
}

