// visiontargeting.h


#ifndef SRC_VISIONTARGETING_H_
#define SRC_VISIONTARGETING_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "Shooter.h"
#include "NetworkTables/networktable.h"


class VisionTargeting
{
public:
	enum Stage { kInitialX, kInitialY, kFinalX, kFinalY };

	VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter);
	~VisionTargeting();
	void Loop();
	bool Targeting() {return (m_targeting > 0);}

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	Shooter *m_shooter;
	shared_ptr<NetworkTable> m_networktable;
	int m_targeting;
	Stage m_stage;
	int m_steady;
};


#endif /* SRC_VISIONTARGETING_H_ */
