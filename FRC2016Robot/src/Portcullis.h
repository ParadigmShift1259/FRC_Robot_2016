// portcullis.h


#ifndef SRC_PORTCULLIS_H_
#define SRC_PORTCULLIS_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "DigitalInput.h"


class Portcullis
{
public:
	enum State { kStored, kLower, kDeployed, kRaise };

	Portcullis(OperatorInputs *inputs);
	~Portcullis();
	void Init();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	SpeedController *m_motor;
	DigitalInput *m_limit;
	State m_state;
	int m_counter;
};


#endif /* SRC_PORTCULLIS_H_ */
