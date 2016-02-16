// visiontargeting.h


#ifndef SRC_VISIONTARGETING_H_
#define SRC_VISIONTARGETING_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "NetworkTables/networktable.h"


class VisionTargeting
{
public:
	VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain);
	~VisionTargeting();
	void Loop();
	bool Targeting() {return (m_targeting > 0);}

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	shared_ptr<NetworkTable> m_networktable;
	int m_targeting;
	bool m_fixy;
};


#endif /* SRC_VISIONTARGETING_H_ */
