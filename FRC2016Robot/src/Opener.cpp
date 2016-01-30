#include <Opener.h>
#include "smartdashboard/smartdashboard.h"
#include <Spark.h>
#include <Const.h>


Opener::Opener(OperatorInputs *inputs)
{
	Spark1 = new Spark(SPARK_OPENER);
	Input = inputs;
	counter = 0;
}


Opener::~Opener()
{
	delete Spark1;
}


void Opener::MoveUp()
{
	if (Input->xBoxXButton())
	{
		Spark1->Set(1);
		counter = 0;
	}
	else
	if (Input->xBoxYButton())
	{
		Spark1->Set(-1);
		counter = 0;
	}
	else
	if (Spark1->Get() != 0)
	{
		counter++;
		if (counter > OPENER_TIMEOUT)
			Spark1->Set(0);
	}
	//SmartDashboard::PutNumber("Opener", counter);
}
