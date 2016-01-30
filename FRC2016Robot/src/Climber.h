// climber.h


#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "Solenoid.h"


class Climber
{
public:
	Climber(OperatorInputs *inputs);
	~Climber();
	void WinchStuff();

protected:
	SpeedController *Spark1;
	Solenoid *PistonActivator;
	OperatorInputs *Input;

};


#endif
