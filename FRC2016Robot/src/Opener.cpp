#include <Opener.h>
#include <Spark.h>
#include <Const.h>

Opener::Opener(OperatorInputs *inputs)
{


	Spark1 = new Spark(SPARK_OPENER);
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

