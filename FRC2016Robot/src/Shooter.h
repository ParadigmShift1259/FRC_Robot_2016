// shooter.h


#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_


#include "OperatorInputs.h"
#include <Solenoid.h>
#include <SpeedController.h>


class Shooter
{
public:
	Shooter(OperatorInputs* inputs);
	~Shooter();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	Solenoid *m_solenoid;
	SpeedController *m_motor;
};


#endif
