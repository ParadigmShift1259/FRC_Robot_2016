// portcullis.cpp


#include <Portcullis.h>
#include <spark.h>
#include "Const.h"


Portcullis::Portcullis(OperatorInputs *operinputs)
{
	inputs = operinputs;
	motor = new Spark(PWM_MOTOR);
	limitin = new DigitalInput(DIO_LIMIT_IN);
	limitout = new DigitalInput(DIO_LIMIT_OUT);
}


Portcullis::~Portcullis()
{
	delete limitin;
	delete limitout;
}


void Portcullis::Loop()
{
	if (inputs->xBoxXButton())
	{
		motor->Set(1);
	}
	else
	if (inputs->xBoxYButton())
	{
		motor->Set(-1);
	}

	if (!limitin->Get())
	{
		motor->Set(0);
	}
	if (!limitout->Get())
	{
		motor->Set(0);
	}
}
