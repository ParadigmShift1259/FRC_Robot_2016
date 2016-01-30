#include "Shooter.h"
#include "Spark.h"
#include "Const.h"


Shooter::Shooter(OperatorInputs* inputs)
{
	wheelMotor = new Spark(SPARK_SHOOTER);
	arm = new Solenoid(ACTUATOR_SOLENOID);
	input = inputs;
}


Shooter::~Shooter()
{
}


void Shooter::ShootBall()
{
	if (input->xBoxRightTrigger())
	{
		arm->Set(true);
	}
	else
	{
		arm->Set(false);
	}
}
