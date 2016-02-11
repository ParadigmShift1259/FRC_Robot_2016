// portcullis.h


#ifndef SRC_PORTCULLIS_H_
#define SRC_PORTCULLIS_H_


#include "SpeedController.h"
#include "Solenoid.h"
#include "OperatorInputs.h"
#include "DigitalInput.h"


class Portcullis
{
public:
	Portcullis(OperatorInputs *operinputs);
	~Portcullis();
	void Loop();

protected:
	OperatorInputs *inputs;
	SpeedController *motor;
	DigitalInput *limitin;
	DigitalInput *limitout;
};


#endif /* SRC_PORTCULLIS_H_ */
