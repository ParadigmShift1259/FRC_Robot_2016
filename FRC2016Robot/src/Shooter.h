// shooter.h


#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_


#include <SpeedController.h>
#include <Solenoid.h>
#include "OperatorInputs.h"


class Shooter
{
public:
	Shooter(OperatorInputs* operinputs);
	~Shooter();
	void Loop();

protected:
	OperatorInputs *inputs;
	Solenoid *arm;
	SpeedController *wheelMotor;
};


#endif
