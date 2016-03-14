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
	enum Stage { /*kInitialX, kInitialY, kFinalX, kFinalY,*/ kReady, kTarget, kStop };

	VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter, Picker *picker);
	~VisionTargeting();
	void Init();
	void Loop(bool target = false, int nocamdelay = 1);
	void Stop();
	bool Targeting() {return ((m_targeting > 0) || m_shoot);}

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	Shooter *m_shooter;
	Picker *m_picker;
	shared_ptr<NetworkTable> m_cvtable;
	int m_counter;
	int m_targeting;
	bool m_shoot;
	double m_leftposition;
	double m_rightposition;
	Stage m_stage;
};


#endif /* SRC_VISIONTARGETING_H_ */
