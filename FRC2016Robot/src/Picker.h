// picker.h


#ifndef SRC_PICKER_H_
#define SRC_PICKER_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "Solenoid.h"
#include <Driverstation.h>


class Picker
{
public:
	enum State { kDown, kShoot1, kShoot2, kUpDelay, kUp, kDownDelay };

	Picker(OperatorInputs* inputs, DriverStation* driverstation);
	~Picker();
	void Init();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	DriverStation *m_driverstation;
	SpeedController *m_motor;
	Solenoid *m_solenoid;
	Solenoid *m_vent;
	State m_state;
	int m_counter;
	bool m_prevreverse;
	double m_stop;
};


#endif /* SRC_PICKER_H_ */
