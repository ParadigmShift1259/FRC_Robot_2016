// shooter.h


#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_


#include "OperatorInputs.h"
#include <Solenoid.h>
#include <SpeedController.h>
#include "DigitalInput.h"


class Shooter
{
public:
	enum Stage { kReady, kRelease, kWinch, kReverse };

	Shooter(OperatorInputs* inputs);
	~Shooter();
	void Loop(bool shoot = false);

protected:
	OperatorInputs *m_inputs;
	Solenoid *m_solenoid;
	SpeedController *m_motor;
	DigitalInput *m_limitdown;
	Stage m_stage;
	int m_counter;
};


#endif
