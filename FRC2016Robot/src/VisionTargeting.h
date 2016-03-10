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
	enum Stage { /*kInitialX, kInitialY, kFinalX, kFinalY,*/ kStop, kLeft, kRight };

	VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter);
	~VisionTargeting();
	void Init();
	void Loop();
	void Stop();
	bool Targeting() {return (m_counter > 0);}

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	Shooter *m_shooter;
	shared_ptr<NetworkTable> m_networktable;
	Stage m_stage;
	int m_counter;
	double m_leftposition;
	double m_rightposition;
};


#endif /* SRC_VISIONTARGETING_H_ */
