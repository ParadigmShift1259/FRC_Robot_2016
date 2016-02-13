// picker.h


#ifndef SRC_PICKER_H_
#define SRC_PICKER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "Solenoid.h"


class Picker
{
public:
	Picker(OperatorInputs* inputs);
	~Picker();
	void Init();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	SpeedController *m_motor;
	Solenoid *m_solenoid;
	bool m_down;
	bool m_prevdown;
	bool m_prevup;
};


#endif /* SRC_PICKER_H_ */
