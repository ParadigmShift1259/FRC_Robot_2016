// climber.h


#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "Solenoid.h"


class Climber
{
public:
	enum State { kDeploy, kDelay, kClimb };

	Climber(OperatorInputs *operatorinputs);
	~Climber();
	void Init();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	SpeedController *m_motor;
	Solenoid *m_solenoid;
	State m_state;
	int m_counter;
};


#endif
