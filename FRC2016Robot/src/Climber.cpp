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
	if(Input->xBoxYButton())
	{
		PistonActivator->Set(true);
	}
	if(Input->xBoxStartButton())
	{
	//	Spark1->Set // Not sure how to use the set thing
	}
}

