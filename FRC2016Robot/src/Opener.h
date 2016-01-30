// opener.h


#ifndef SRC_OPENER_H_
#define SRC_OPENER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"


class Opener
{
public:
	Opener(OperatorInputs *operinputs);
	~Opener();
	void Loop();

protected:
	OperatorInputs *inputs;
	SpeedController *Spark1;
	int counter;
};


#endif
