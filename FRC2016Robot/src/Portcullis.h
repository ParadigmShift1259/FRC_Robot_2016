// portcullis.h


#ifndef SRC_PORTCULLIS_H_
#define SRC_PORTCULLIS_H_


#include "OperatorInputs.h"
#include "SpeedController.h"
#include "DigitalInput.h"


class Portcullis
{
public:
	Portcullis(OperatorInputs *inputs);
	~Portcullis();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	SpeedController *m_motor;
	DigitalInput *m_limitdown;
	DigitalInput *m_limitup;
};


#endif /* SRC_PORTCULLIS_H_ */
