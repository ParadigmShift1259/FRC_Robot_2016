#ifndef SRC_OPENER_H_
#define SRC_OPENER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"


class Opener
{
public:
	Opener(OperatorInputs *inputs);
	~Opener();
	void MoveUp();

protected:
	SpeedController *Spark1;
	OperatorInputs *Input;
	int counter;
};


#endif
