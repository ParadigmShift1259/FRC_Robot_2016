// shooter.h


#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_


#include "OperatorInputs.h"
#include "Picker.h"
#include <Solenoid.h>
#include <SpeedController.h>
#include <DigitalInput.h>
#include <Encoder.h>


class Shooter
{
public:
	enum Stage { kReady, kPickerDrop, kRelease, kWinch, kLock, kReverse, kOverride };

	Shooter(OperatorInputs* inputs, Picker* picker);
	~Shooter();
	void Init();
	void Loop(bool shoot = false);
	Stage GetStage() { return m_stage; }

protected:
	OperatorInputs *m_inputs;
	Picker *m_picker;
	Solenoid *m_solenoid;
	SpeedController *m_motor;
	DigitalInput *m_limitdown;
	Encoder *m_encoder;
	Stage m_stage;
	int m_counter;
};


#endif
