




Shooter::Shooter(OperatorInputs inputs)
{
	wheelMotor = new Spark(4);
	arm = new Solenoid(4);
	input = inputs;
}

Shooter::~Shooter()
{


}

void Shooter::ShootBall()
{
	if(input->xBoxRightBumper())
	{
		arm->Set(true);
	}
}
