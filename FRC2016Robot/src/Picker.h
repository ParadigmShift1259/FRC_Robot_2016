// picker.h


#ifndef SRC_PICKER_H_
#define SRC_PICKER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "Solenoid.h"


class Picker
{
public:
	enum State { kDown, kShoot1, kShoot2, kUpDelay, kUp };

	Picker(OperatorInputs* inputs);
	~Picker();
	void Init();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	SpeedController *m_motor;
	Solenoid *m_solenoid;
	State m_state;
	int m_counter;
	bool m_prevreverse;
};


#endif /* SRC_PICKER_H_ */
