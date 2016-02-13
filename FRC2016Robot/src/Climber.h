// climber.h


#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "Solenoid.h"


class Climber
{
public:
	Climber(OperatorInputs *operatorinputs);
	~Climber();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	SpeedController *m_motor;
	Solenoid *m_solenoid;
	bool m_deploytoggle;
};


#endif
