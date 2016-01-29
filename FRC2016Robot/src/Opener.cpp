#include <Opener.h>
#include <Spark.h>

Opener::Opener(OperatorInputs *inputs)
{


	Spark1 = new Spark(0);
	Input = inputs;

}

Opener::~Opener()
{
	delete Spark1;

}

void Opener::MoveUp()
{
	if(Input->xBoxXButton())
	{
		Spark1->Set(1);
	}
	if(Input->xBoxYButton())
	{
		Spark1->Set(-1);
	}
}

